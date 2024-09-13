#pragma once
#include "utils/Demangle.h"
#include "utils/memory/Ref.h"
#include "utils/threading/System.h"
#include <vector>

namespace Espresso::Threading
{
	class SystemManager
	{
	public:
		template <typename T> auto AddSystem() -> T*
		{
			if (Internals::typeCheck<System, T>())
			{
				return nullptr;
			}

			auto system = makeRef<T>();
			_systems.push_back(system);
		}

		inline void Run()
		{
			for (auto& system : _systems)
			{
				system->Initialize();
			}
		}

		inline void Shutdown()
		{
			for (auto& system : _systems)
			{
				system->Running.store(false);
			}
		}

	private:
		std::vector<Ref<System>> _systems;
	};
}