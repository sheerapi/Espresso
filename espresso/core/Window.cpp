#include "Window.h"
#include "core/Logger.h"
#include "graphics/Renderer.h"

namespace Espresso
{
	Window::Window(const std::string& title, int width, int height)
	{
		_title = title;
		_width = width;
		_height = height;

		_window =
			SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
							 SDL_WINDOWPOS_UNDEFINED, width, height,
							 SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE |
								 Graphics::Renderer::main->GetInternalBackendFlag());

		if (_window == nullptr)
		{
			es_coreError("Failed to create window");
		}
	}

	Window::~Window()
	{
		Close();
	}

	void Window::Close()
	{
		if (!_closed)
		{
			SDL_DestroyWindow(_window);
		}
	}
}