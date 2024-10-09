#pragma once
#include "SDL_events.h"
#include "SDL_video.h"
#include "utils/EventHandler.h"
#include "utils/math/Vector2.h"
#include <string>

namespace Espresso
{
	class Window
	{
	public:
		Window(const std::string& title = "App", int width = 1280, int height = 720);

		~Window();
		void Close();

		[[nodiscard]] inline auto GetWidth() const -> int
		{
			return (int)_size.X;
		}

		[[nodiscard]] inline auto GetHeight() const -> int
		{
			return (int)_size.Y;
		}

		[[nodiscard]] inline auto GetAspectRatio() const -> float
		{
			return (float)GetWidth() / (float)GetHeight();
		}

		[[nodiscard]] inline auto GetTitle() const -> std::string
		{
			return _title;
		}

		[[nodiscard]] inline auto GetHandle() const -> void*
		{
			return (void*)_window;
		}

		[[nodiscard]] inline auto GetPosition() const -> Vector2
		{
			return _pos;
		}

	private:
		SDL_Window* _window;
		std::string _title;
		Vector2 _size;
		Vector2 _pos;
		bool _closed{false};

		void _handleEvent(SDL_Event* e);

		friend class Internals::EventHandler;
	};
}