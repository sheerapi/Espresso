#pragma once
#include "core/EventManager.h"
#include "core/Logger.h"
#include "utils/Demangle.h"
#include "utils/threading/System.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
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

			if (systems.size() >= std::thread::hardware_concurrency())
			{
				es_coreWarn("Hardware thread softlimit reached, consider stopping some "
							"systems... ({} systems)",
							systems.size());
			}

			auto system = std::make_shared<T>();
			systems.push_back(system);

			es_coreTrace("Allocating new {} instance...({} systems)",
						 Internals::demangle<T>(), systems.size() - 1);

			return std::shared_ptr<T>(system);
		}

		static inline auto GetSystem(int index) -> std::shared_ptr<System>
		{
			es_coreAssert((int)systems.size() > index, "Out of boundaries");

			return {systems[index]};
		}

		inline static void Run()
		{
			for (auto& system : systems)
			{
				system->Initialize();
			}

			std::lock_guard<std::mutex> lock(mtx);
			ready = true;
			running = true;
			cv.notify_all();
		}

		inline static void Shutdown()
		{
			std::unique_lock<std::mutex> lock(mtx);
			running = false;

			for (auto& system : systems)
			{
				EventManager::TriggerEvent(
					"system.shutdown",
					(void*)Internals::demangle(typeid(*system).name()).c_str()); // NOLINT
			}

			es_coreDebug("Waiting for threads to terminate...");
			cv.wait(lock, []() { return shutdowns.load() == (int)systems.size(); });

			systems.clear();
		}

		inline static auto GetCV() -> std::condition_variable&
		{
			return cv;
		}

		inline static auto Ready() -> bool
		{
			return ready;
		}

		inline static auto Running() -> bool
		{
			return running;
		}

		inline static void ReportShutdown()
		{
			shutdowns++;
		}

	private:
		inline static std::vector<std::shared_ptr<System>> systems;
		inline static std::mutex mtx;
		inline static std::condition_variable cv;
		inline static std::atomic<bool> ready;
		inline static std::atomic<bool> running;
		inline static std::atomic<int> shutdowns;
	};
}