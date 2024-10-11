#pragma once
#include "SDL_platform.h"
#include "core/ApplicationInfo.h"
#include "core/Window.h"
#include "utils/threading/ThreadTime.h"
#include <memory>
#include <string>

#define es_sdlInitFlag                                                                   \
	SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK |    \
		SDL_INIT_TIMER | SDL_INIT_SENSOR | SDL_INIT_VIDEO

namespace Espresso
{
	namespace Internals
	{
		class EventHandler;
	}

	class Application
	{
	public:
		inline static Application* main{nullptr};

		Application();
		~Application();

		auto CreateWindow(const std::string& title = "App", int width = 1280,
						  int height = 720) -> std::shared_ptr<Window>;

		inline auto GetWindow() -> std::shared_ptr<Window>
		{
			return _window;
		}

		[[nodiscard]] static inline auto QueryPlatform() -> std::string
		{
			return SDL_GetPlatform();
		}

		auto Present() -> bool;

		[[nodiscard]] inline auto GetDelta() const -> double
		{
			return _time->GetDelta();
		}

		[[nodiscard]] inline auto GetElapsed() const -> double
		{
			return _time->GetElapsed();
		}

		[[nodiscard]] inline auto GetFrames() const -> unsigned long
		{
			return _time->GetFrames();
		}

		[[nodiscard]] inline static auto GetAppInfo() -> ApplicationInfo
		{
			return main->_info;
		}

		[[nodiscard]] inline static auto GetAppID() -> ApplicationID
		{
			return main->_info.AppID;
		}

		[[nodiscard]] inline static auto GetEnvInfo() -> EnvironmentInfo
		{
			return main->_env;
		}

		inline void Close()
		{
			_running = false;
		}

	private:
		bool _running{false};

		ApplicationInfo _info;
		EnvironmentInfo _env;

		std::shared_ptr<Window> _window{nullptr};
		std::shared_ptr<Threading::ThreadTime>
			_time; // constructor calls functions that are not ready yet

		void _setupEnvInfo();

		friend class Internals::EventHandler;
		friend class AssetManager;
	};
}