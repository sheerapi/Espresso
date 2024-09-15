#pragma once
#include "ThreadTime.h"
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace Espresso::Threading
{
	using ThreadID = std::thread::id;

	class System
	{
	public:
		void Initialize();

		virtual ~System() = default;

		void Enqueue(const std::function<void()>& work)
		{
			std::lock_guard<std::mutex> lock(Mutex);
			_workQueue.push(work);
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
		virtual void Tick() {};
		virtual void Shutdown() {};
		virtual void Run();
		void ExecuteWorkQueue();

		ThreadTime Time;
		std::mutex Mutex;

	private:
		std::thread _thread;
		std::queue<std::function<void()>> _workQueue;
	};
}