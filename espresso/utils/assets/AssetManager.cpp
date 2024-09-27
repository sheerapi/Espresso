#include "AssetManager.h"
#include "config.h"
#include "core/Application.h"
#include "core/Logger.h"
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
		rapidjson::Document registryDoc;

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

		registryDoc.Parse(Read("registry.json").data());

		es_coreAssert(registryDoc.IsObject(), "Expected an object");
		es_coreAssert(registryDoc.HasMember("assets"), "Expected assets object");
		es_coreAssert(registryDoc["assets"].IsObject(), "Expected an object");

		for (rapidjson::Value::ConstMemberIterator itr =
				 registryDoc["assets"].MemberBegin();
			 itr != registryDoc["assets"].MemberEnd(); ++itr)
		{
			es_coreAssert(itr->name.IsString(), "Expected a string");
			es_coreAssert(itr->value.IsString(), "Expected a string");

			registry.insert({std::string(itr->name.GetString()),
							 std::string(itr->value.GetString())});
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