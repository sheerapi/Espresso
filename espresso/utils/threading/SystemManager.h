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
		template <typename T> static auto AddSystem() -> T*
		{
			if (Internals::typeCheck<System, T>())
			{
				return nullptr;
			}

			auto system = makeRef<T>();
			systems.push_back(system);
		}

		inline static void Run()
		{
			for (auto& system : systems)
			{
				system->Initialize();
			}
		}

		inline static void Shutdown()
		{
			for (auto& system : systems)
			{
				system->Running.store(false);
			}
		}

	private:
		inline static std::vector<Ref<System>> systems;
	};
}