#include "TickSystem.h"
#include "core/Scene.h"

namespace Espresso::Threading
{
	void TickSystem::Tick()
	{
		Scene::main->Tick();
	}

	void TickSystem::Shutdown()
	{
		delete Scene::main;
	}
}