#include "EventHandler.h"
#include "SDL_events.h"
#include "core/Application.h"
#include "core/Logger.h"
#include "input/Input.h"

namespace Espresso::Internals
{
	void EventHandler::HandleEvent(SDL_Event* event)
	{
		switch ((SDL_EventType)event->type)
		{
		case SDL_QUIT:
			Application::main->_running = false;
		case SDL_APP_TERMINATING:
			Application::main->_running = false;
			break;

		case SDL_APP_LOWMEMORY:
			es_coreWarn("Application running low on memory!");
			break;

		case SDL_WINDOWEVENT:
			Application::main->_window->_handleEvent(event);
			break;

		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
		case SDL_DROPFILE:
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
		case SDL_CONTROLLERTOUCHPADMOTION:
		case SDL_CONTROLLERTOUCHPADDOWN:
		case SDL_CONTROLLERTOUCHPADUP:
		case SDL_CONTROLLERSENSORUPDATE:
		case SDL_CONTROLLERDEVICEREMOVED:
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
		case SDL_JOYBALLMOTION:
		case SDL_JOYBUTTONDOWN:
		case SDL_JOYBUTTONUP:
		case SDL_JOYHATMOTION:
		case SDL_JOYBATTERYUPDATED:
			Input::Handle((void*)event);
			break;

		default:
			break;
		}
	}
}