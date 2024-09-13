#pragma once
#include "SDL_platform.h"
#include "core/ApplicationInfo.h"
#include "core/Window.h"
#include <memory>
#include <string>

#define es_sdlInitFlag                                                                   \
	SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC | SDL_INIT_JOYSTICK |    \
		SDL_INIT_TIMER | SDL_INIT_SENSOR | SDL_INIT_VIDEO

namespace Espresso
{
	class Application
	{
	public:
		inline static Application* main{nullptr};

		Application(const std::string& appId);
		~Application();

		auto CreateWindow(const std::string& title = "App", int width = 1280,
						  int height = 720) -> std::shared_ptr<Window>;

		[[nodiscard]] static inline auto QueryPlatform() -> std::string
		{
			return SDL_GetPlatform();
		}

		auto Present() -> bool;

		[[nodiscard]] inline auto GetDelta() const -> double
		{
			return _deltaTime;
		}

		[[nodiscard]] inline static auto GetAppID() -> ApplicationID
		{
			return main->_id;
		}

		[[nodiscard]] inline static auto GetEnvInfo() -> EnvironmentInfo
		{
			return main->_env;
		}

		[[nodiscard]] inline auto GetTPS() const -> double
		{
			double safeTPS = 0;

			if (GetDelta() > 0)
			{
				if (GetDelta() < 1e-6F)
				{
					safeTPS = 1.0F / 1e-6F;
				}
				else
				{
					safeTPS = 1.0F / GetDelta();
				}
			}
			else
			{
				safeTPS = 0;
			}

			return safeTPS;
		}

	private:
		double _deltaTime;
		bool _running{false};

		ApplicationID _id;
		EnvironmentInfo _env;

		std::shared_ptr<Window> _window;

		void _setupAppId(const std::string& appId);
		void _setupEnvInfo();
	};
}