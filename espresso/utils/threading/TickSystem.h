#pragma once
#include "System.h"
#include "SystemManager.h" // IWYU pragma: keep

namespace Espresso::Threading
{
	class TickSystem : public System
	{
	public:
		void Tick() override;
		void Shutdown() override;
	};
}

#define es_tickSystem Espresso::Threading::SystemManager::GetSystem(0)