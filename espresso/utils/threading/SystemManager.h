#pragma once
#include "core/Logger.h"
#include "utils/Demangle.h"
#include "utils/threading/System.h"
#include <memory>
#include <vector>

namespace Espresso::Threading
{
	class SystemManager
	{
	public:
		template <typename T> static auto AddSystem() -> std::shared_ptr<T>
		{
			if (!Internals::typeCheck<System, T>())
			{
				return std::shared_ptr<T>();
			}

			auto system = std::make_shared<T>();
			systems.push_back(system);

			return std::shared_ptr<T>(system);
		}

		static inline auto GetSystem(int index) -> std::shared_ptr<System>
		{
			es_coreAssert((int)systems.size() > index, "Out of boundaries");

			return systems[index];
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
		inline static std::vector<std::shared_ptr<System>> systems;
	};
}