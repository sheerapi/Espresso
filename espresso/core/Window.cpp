#include "Window.h"
#include "core/EventManager.h"
#include "core/Logger.h"
#include "graphics/Renderer.h"

namespace Espresso
{
	Window::Window(const std::string& title, int width, int height)
	{
		_title = title;
		_size = Vector2((float)width, (float)height);

		_window =
			SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
							 SDL_WINDOWPOS_UNDEFINED, width, height,
							 SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE |
								 Graphics::Renderer::main->GetInternalBackendFlag());

		if (_window == nullptr)
		{
			es_coreError("Failed to create window");
		}

		int x;
		int y;

		SDL_GetWindowPosition(_window, &x, &y);

		_pos = Vector2((float)x, (float)y);
	}

	Window::~Window()
	{
		Close();
	}

	void Window::_handleEvent(SDL_Event* e)
	{
		switch ((SDL_WindowEventID)e->window.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
		{
			Close();
			EventManager::TriggerEvent("quit");
			break;
		}

		case SDL_WINDOWEVENT_SHOWN:
			EventManager::TriggerEvent("window.shown");
			break;

		case SDL_WINDOWEVENT_HIDDEN:
			EventManager::TriggerEvent("window.hidden");
			break;

		case SDL_WINDOWEVENT_EXPOSED:
			EventManager::TriggerEvent("window.shown");
			break;

		case SDL_WINDOWEVENT_MOVED:
		{
			_pos = Vector2((float)e->window.data1, (float)e->window.data2);
			EventManager::TriggerEvent(
				"window.moved",
				new Vector2((float)e->window.data1, (float)e->window.data2));
			break;
		}

		case SDL_WINDOWEVENT_SIZE_CHANGED:
		case SDL_WINDOWEVENT_RESIZED:
		{
			_pos = Vector2((float)e->window.data1, (float)e->window.data2);
			EventManager::TriggerEvent(
				"window.resized",
				new Vector2((float)e->window.data1, (float)e->window.data2));
			break;
		}

		case SDL_WINDOWEVENT_ENTER:
		{
			EventManager::TriggerEvent("mouse.enterwindow");
			break;
		}

		case SDL_WINDOWEVENT_LEAVE:
		{
			EventManager::TriggerEvent("mouse.leavewindow");
			break;
		}

		case SDL_WINDOWEVENT_FOCUS_GAINED:
		{
			EventManager::TriggerEvent("window.focused", new bool{true});
			break;
		}

		case SDL_WINDOWEVENT_FOCUS_LOST:
		{
			EventManager::TriggerEvent("window.focused", new bool{false});
			break;
		}

		case SDL_WINDOWEVENT_DISPLAY_CHANGED:
		{
			EventManager::TriggerEvent("window.displaychanged", &e->window.data1);
			break;
		}
		default:
			break;
		}
	}

	void Window::Close()
	{
		if (!_closed)
		{
			SDL_DestroyWindow(_window);
		}
	}
}