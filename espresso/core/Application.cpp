#include "Application.h"
#include "SDL.h"
#include "core/EventManager.h"
#include "core/Logger.h"
#include "core/Scene.h"
#include "utils/EventHandler.h"
#include "utils/StringUtils.h"
#include "utils/assets/AssetManager.h"
#include "utils/threading/Scheduler.h"
#include "utils/threading/SystemManager.h"
#include "utils/threading/TickSystem.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <thread>

#ifdef DEBUG
#	include <regex>
#endif

namespace Espresso
{
	Application::Application()
	{
		if (main != nullptr)
		{
			es_coreError("Another instance of Espresso::Application exists!");
		}

		main = this;

		Threading::Scheduler::Init();

		_setupEnvInfo();
		AssetManager::Init();

		if (SDL_Init(es_sdlInitFlag) < 0)
		{
			es_coreFatal("SDL Failed to initialize. Cannot continue.");
			return;
		}

		Threading::SystemManager::AddSystem<Threading::TickSystem>();

		Scene::ChangeScene(new Scene("MainScene"));

		es_coreInfo("Initialized {}!", _info.AppID.GetCompoundID());
	}

	auto Application::Present() -> bool
	{
		if (_running)
		{
			es_coreError("Already presented!");
			return false;
		}

		_running = true;

		Threading::SystemManager::Run();
		EventManager::TriggerEvent("present");

		SDL_Event e;

		_time = std::make_shared<Threading::ThreadTime>();

		while (_running)
		{
			_time->StartMeasure();

			while (SDL_PollEvent(&e) == 1)
			{
				Internals::EventHandler::HandleEvent(&e);
			}

			_time->EndMeasure();

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		return true;
	}

	auto Application::CreateWindow(const std::string& title, int width,
								   int height) -> std::shared_ptr<Window>
	{
		_window = std::make_shared<Window>(title, width, height);
		return {_window};
	}

	Application::~Application()
	{
		Threading::SystemManager::Shutdown();
		Threading::Scheduler::Shutdown();
		SDL_Quit();
		es_coreWarn("Bye!");
	}

	ApplicationID::ApplicationID(const std::string& appId)
	{
#ifdef DEBUG
		if (!std::regex_match(
				appId, std::regex("[a-zA-Z0-9_-]+\\.[a-zA-Z0-9_-]+(\\.[a-zA-Z0-9_-]+)?")))
		{
			es_coreError("Application ID \"{}\" is invalid", appId);
			return;
		}
#endif

		auto segments = stringSplit(appId, ".");

		if (segments.size() == 3)
		{
			Domain = segments[0];
			Org = segments[1];
			Name = segments[2];
		}
		else
		{
			Org = segments[0];
			Name = segments[1];
		}
	}

	void Application::_setupEnvInfo()
	{
		_env.WorkingDirectory = std::filesystem::current_path();

		char buffer[1024];
		std::string path;

#if defined(_WIN32) || defined(_WIN64)
		DWORD size = GetModuleFileNameA(NULL, buffer, sizeof(buffer));
		if (size > 0 && size < sizeof(buffer))
		{
			path = std::string(buffer, size);
		}
		else
		{
			es_coreError("Unable to get executable path on Windows.");
		}
#elif defined(__APPLE__)
		uint32_t size = sizeof(buffer);
		if (_NSGetExecutablePath(buffer, &size) == 0)
		{
			path = std::string(buffer);
		}
		else
		{
			std::vector<char> largerBuffer(size);
			if (_NSGetExecutablePath(largerBuffer.data(), &size) == 0)
			{
				path = std::string(largerBuffer.data(), size);
			}
			else
			{
				es_coreError("Unable to get executable path on macOS.");
			}
		}
#elif defined(__linux__)
		ssize_t size = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
		if (size != -1)
		{
			buffer[size] = '\0';
			path = std::string(buffer);
		}
		else
		{
			es_coreError("Unable to get executable path on Linux.");
		}
#else
		path = std::filesystem::path(_env.WorkingDirectory) / _id.Name;
		es_coreWarn("Unsupported platform at _setupEnvInfo()");
#endif

		_env.ExecutablePath = path;

#if defined(_WIN32) || defined(_WIN64)
		LPWSTR* szArglist;
		int nArgs;
		szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);
		if (szArglist)
		{
			for (int i = 0; i < nArgs; ++i)
			{
				std::wstring ws(szArglist[i]);
				_env.CommandLineArgs.push_back(std::string(ws.begin(), ws.end()));
			}
			LocalFree(szArglist);
		}
		else
		{
			es_coreError("Unable to get command line arguments on Windows.");
		}
#elif defined(__APPLE__)
		int argc = *_NSGetArgc();
		char** argv = *_NSGetArgv();
		for (int i = 0; i < argc; ++i)
		{
			_env.CommandLineArgs.push_back(std::string(argv[i]));
		}
#elif defined(__linux__)
		std::ifstream cmdline("/proc/self/cmdline");
		if (cmdline)
		{
			std::string arg;
			while (std::getline(cmdline, arg, '\0'))
			{
				_env.CommandLineArgs.push_back(arg);
			}
		}
		else
		{
			es_coreError("Unable to get command line arguments on Linux.");
		}
#else
		es_coreError("Unsupported platform at _setupEnvInfo().");
#endif

#ifndef DEBUG
		_env.AssetsDirectory =
			std::filesystem::path(_env.ExecutablePath).remove_filename() / "assets";
#else
		_env.AssetsDirectory =
			std::filesystem::path(_env.ExecutablePath).remove_filename() / "assets.pak";
#endif

		_env.RootPath = std::filesystem::path(_env.ExecutablePath).remove_filename();
	}
}