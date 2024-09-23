#include "AssetManager.h"
#include "config.h"
#include "core/Application.h"
#include "core/Logger.h"
#include "utils/StringUtils.h"
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
	}
}