#pragma once
#include "SDL_events.h"

namespace Espresso::Internals
{
	class EventHandler
	{
	public:
		static void HandleEvent(SDL_Event* event);
	};
}