#pragma once
#include "SDL_video.h"
#include "core/Object.h"
#include <string>

namespace Espresso
{
	class Window : public Object
	{
	public:
		Window(const std::string& title = "App", int width = 1280, int height = 720);

		~Window();
		void Close();

		[[nodiscard]] inline auto GetWidth() const -> int
		{
			return _width;
		}

		[[nodiscard]] inline auto GetHeight() const -> int
		{
			return _height;
		}

		[[nodiscard]] inline auto GetAspectRatio() const -> float
		{
			return (float)_width / (float)_height;
		}

		[[nodiscard]] inline auto GetTitle() const -> std::string
		{
			return _title;
		}

		[[nodiscard]] auto ToString() const -> std::string override
		{
			return std::format("[{} ._handle = {}, ._closed = {}, ._w = {}, ._h = {}, "
							   "._title = {} ({})]",
							   GetName(), (void*)_window, _closed, _width, _height,
							   _title, static_cast<const void*>(this));
		}

	private:
		SDL_Window* _window;
		std::string _title;
		int _width;
		int _height;
		bool _closed{false};
	};
}