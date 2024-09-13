#pragma once
#include "ThreadTime.h"
#include <atomic>
#include <functional>
#include <queue>
#include <string>
#include <thread>

namespace Espresso::Threading
{
	using ThreadID = std::thread::id;

	class System
	{
	public:
		std::atomic_bool Running;

		void Initialize();

		[[nodiscard]] virtual inline auto GetName() const -> std::string
		{
			return "System";
		}

		[[nodiscard]] virtual inline auto GetTime() const -> ThreadTime
		{
			return Time;
		}

		[[nodiscard]] virtual inline auto GetID() const -> unsigned long
		{
			return std::hash<std::thread::id>()(_thread.get_id());
		}

	protected:
		virtual void Init() {};
		virtual void Shutdown() {};
		virtual void Tick() = 0;
		virtual void Run();
		void ExecuteWorkQueue();

		ThreadTime Time;

	private:
		std::thread _thread;
		std::queue<std::function<void()>> _workQueue;
	};
}