#pragma once
#include "SDL.h"
#include "core/ApplicationInfo.h"
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

		void _setupAppId(const std::string& appId);
		void _setupEnvInfo();
	};
}