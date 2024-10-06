#pragma once
#include "System.h"
#include "SystemManager.h" // IWYU pragma: keep

namespace Espresso::Threading
{
	class RenderSystem : public System
	{
	public:
		void Tick() override;
		void Shutdown() override;
		void Init() override;
	};
}

#define es_renderSystem Espresso::Threading::SystemManager::GetSystem(1)