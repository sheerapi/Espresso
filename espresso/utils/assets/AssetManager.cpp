#include "AssetManager.h"
#include "config.h"
#include "core/Application.h"
#include "core/Logger.h"
#include "tools/SHA256.h"
#include "tools/base64.hpp"
#include "utils/StringUtils.h"
#include <sstream>
#define RAPIDJSON_NOMEMBERITERATORCLASS 1
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <filesystem>
#include <fstream>

namespace Espresso
{
	void AssetManager::Init()
	{
		rapidjson::Document projectDoc;

#ifndef DEBUG
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
#endif
	}
}