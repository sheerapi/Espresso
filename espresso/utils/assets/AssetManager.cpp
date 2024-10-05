#include "AssetManager.h"
#include "config.h"
#include "core/Application.h"
#include "core/Logger.h"

#ifndef DEBUG
#	include "tools/SHA256.h"
#	include "tools/base64.hpp"
#endif

#include "utils/StringUtils.h"
#include <cstdio>
#include <sstream>
#define RAPIDJSON_NOMEMBERITERATORCLASS 1
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <filesystem>
#include <fstream>

#define es_chunkSize 16384

namespace Espresso
{
	void AssetManager::Init()
	{
		rapidjson::Document projectDoc;

#ifdef DEBUG
		if (!std::filesystem::exists(std::filesystem::path(Application::GetEnvInfo().RootPath) / "project.json"))
		{
			es_coreError("project.json file not found!");
			return;
		}

		auto file = std::ifstream(
			std::filesystem::path(Application::GetEnvInfo().RootPath) / "project.json");
		rapidjson::IStreamWrapper isw(file);

		projectDoc.ParseStream(isw);
#else
		if (!std::filesystem::exists(
				std::filesystem::path(Application::GetEnvInfo().RootPath) / "project"))
		{
			es_coreError("project file not found!");
			return;
		}

		auto file = std::ifstream(
			std::filesystem::path(Application::GetEnvInfo().RootPath) / "project",
			std::ios::binary);

		std::stringstream ss;
		ss << file.rdbuf();

		auto content = ss.str();

		for (auto& c : content)
		{
			c = static_cast<char>(static_cast<unsigned char>(c) >> 1);
		}

		content = base64::from_base64(content);

		auto shaFile = content.substr(0, 64);

		content = content.substr(64);

		SHA256 sha;
		sha.update(content);

		if (SHA256::toString(sha.digest()) != shaFile)
		{
			es_coreError("Checksums for project file do not match!");
			return;
		}

		projectDoc.Parse(content.c_str());
#endif

		es_coreAssert(projectDoc.IsObject(), "Expected an object");
		es_coreAssert(projectDoc.HasMember("project"), "Expected project object");
		es_coreAssert(projectDoc["project"].IsObject(),
					  "Expected object at project member");
		es_coreAssert(projectDoc["project"].HasMember("name"), "Expected name member");
		es_coreAssert(projectDoc["project"].HasMember("author"),
					  "Expected author member");

		es_coreAssert(projectDoc["project"]["name"].IsString(),
					  "Expected string at name");
		es_coreAssert(projectDoc["project"]["author"].IsString(),
					  "Expected string at author");

		Application::main->_info.Name = projectDoc["project"]["name"].GetString();
		Application::main->_info.Organization =
			projectDoc["project"]["author"].GetString();

		if (projectDoc["project"].HasMember("version"))
		{
			es_coreAssert(projectDoc["project"]["version"].IsString(),
						  "Expected string at version");
			Application::main->_info.Version =
				projectDoc["project"]["version"].GetString();
		}
		else
		{
			Application::main->_info.Version = "1.0.0";
		}

		if (projectDoc["project"].HasMember("id"))
		{
			es_coreAssert(projectDoc["project"]["id"].IsString(),
						  "Expected string at id");
			Application::main->_info.AppID =
				ApplicationID(projectDoc["project"]["id"].GetString());
		}
		else
		{
			Application::main->_info.AppID =
				ApplicationID(stringToLower(Application::GetAppInfo().Organization + "." +
											Application::GetAppInfo().Name));
		}

#ifdef DEBUG
		for (const auto& entry : std::filesystem::recursive_directory_iterator("assets"))
		{
			if (entry.is_regular_file())
			{
				auto path = std::filesystem::relative(entry.path(), "assets");

				path.replace_extension("");

				if (registry.contains(path))
				{
					es_coreWarn("Asset \"{}\" already exists!", path.string());
				}

				registry[path] = std::filesystem::relative(entry.path(), "assets");
			}
		}
#else
		auto package =
			std::ifstream(Application::GetEnvInfo().AssetsDirectory, std::ios::binary);

		while (package.peek() != EOF)
		{
			unsigned char pathLength;
			std::string path;

			package.read(reinterpret_cast<char*>(&pathLength), sizeof(pathLength));

			path.resize(pathLength + 1);

			package.read(reinterpret_cast<char*>(path.data()), (int)pathLength);

			unsigned offset = package.tellg();
			registryOffsets[path] = offset;

			uint32_t fileSize;
			package.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

			package.seekg(fileSize, std::ios::cur);

			if (registry.contains(std::filesystem::path(path).replace_extension("")))
			{
				es_coreWarn("Asset \"{}\" already exists!", path);
			}

			registry[std::filesystem::path(path).replace_extension("")] = path;
		}

		dctx = ZSTD_createDCtx();
		ZSTD_DCtx_setParameter(dctx, ZSTD_d_windowLogMax, 18);
		ZSTD_DCtx_setParameter(dctx, ZSTD_d_stableOutBuffer, 1);
#endif

		es_coreInfo("Loaded {} asset entries!", registry.size());
	}

	auto AssetManager::Read(const std::string& path) -> std::string
	{
#ifdef DEBUG
		auto fullPath =
			std::filesystem::path(Application::GetEnvInfo().AssetsDirectory) / path;

		if (!std::filesystem::exists(fullPath))
		{
			es_coreError("{} does not exist!", fullPath.string());
			return "";
		}

		std::ifstream file(fullPath, std::ios_base::binary);
		std::stringstream stream;

		stream << file.rdbuf();

		return stream.str();
#else
		if (!registryOffsets.contains(path))
		{
			es_coreError("{} does not exist!", path);
			return "";
		}

		std::vector<char> compressedBuffer(es_chunkSize);
		std::vector<char> decompressedBuffer(es_chunkSize);

		auto package =
			std::ifstream(Application::GetEnvInfo().AssetsDirectory, std::ios::binary);

		package.seekg(registryOffsets[path], std::ios::cur);

		uint32_t fileSize;
		package.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

		ZSTD_outBuffer zstdOut{decompressedBuffer.data(), decompressedBuffer.size(), 0};
		ZSTD_inBuffer zstdIn{compressedBuffer.data(), 0, 0};

		std::ostringstream output(std::stringstream::binary);

		while (fileSize > 0 && package.peek() != EOF)
		{
			package.read(compressedBuffer.data(),
						 (fileSize > es_chunkSize ? es_chunkSize : fileSize));
			zstdIn.src = compressedBuffer.data();
			zstdIn.size = package.gcount();
			zstdIn.pos = 0;

			while (zstdIn.pos < zstdIn.size)
			{
				zstdOut.pos = 0;
				size_t result = ZSTD_decompressStream(dctx, &zstdOut, &zstdIn);
				if (ZSTD_isError(result) == 1U)
				{
					es_coreError("Failed to decompress {}! {}", path,
								 ZSTD_getErrorName(result));
					return "";
				}

				output.write(decompressedBuffer.data(), static_cast<long>(zstdOut.pos));
				fileSize -= zstdOut.pos;
			}
		}

		return output.str();
#endif
	}
}