#pragma once
#include "ThreadTime.h"
#include <atomic>
#include <functional>
#include <queue>
#include <string>
#include <thread>

namespace Espresso::Threading
{
	class System
	{
	public:
		std::atomic_bool Running;

		virtual void Init() {};
		virtual void Shutdown() {};
		virtual void Tick() = 0;

		[[nodiscard]] virtual inline auto GetName() const -> std::string
		{
			return "System";
		}

		[[nodiscard]] virtual inline auto GetTime() const -> ThreadTime
		{
			return Time;
		}

	protected:
		virtual void Run();
		void ExecuteWorkQueue();

		ThreadTime Time;

	private:
		std::thread _thread;
		std::queue<std::function<void()>> _workQueue;
	};
}