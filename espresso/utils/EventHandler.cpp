#include "EventHandler.h"
#include "core/Application.h"
#include "core/Logger.h"

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

		default:
			break;
		}
	}
}