#pragma once
#include "ThreadTime.h"
#include "core/Object.h"
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace Espresso::Threading
{
	using ThreadID = std::thread::id;

	class System : public Object
	{
	public:
		void Initialize();

		virtual ~System() = default;

		void Enqueue(const std::function<void()>& work)
		{
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

		[[nodiscard]] auto ToString() const -> std::string override
		{
			return std::format("[{} ._thread = {}, ._workQueue = {} ({}, {} frames)]",
							   GetName(), GetID(), _workQueue.size(),
							   static_cast<const void*>(this), Time.GetFrames());
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