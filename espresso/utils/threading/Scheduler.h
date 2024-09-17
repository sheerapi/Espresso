#pragma once
#include "utils/threading/ThreadTime.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace Espresso::Threading
{
	class Task
	{
	public:
		Task(const std::function<void()>& task) : _task(task) {};

		inline void OnFinish(const std::function<void()>& callback)
		{
			_callback = callback;
		}

		auto IsFinished() -> bool
		{
			return _finished;
		}

	private:
		std::function<void()> _task;
		std::function<void()> _callback{nullptr};
		std::atomic<bool> _finished;

		inline void _run()
		{
			_task();
			_finished = true;

			if (_callback != nullptr)
			{
				_callback();
			}
		}

		friend class Scheduler;
	};

	class Scheduler
	{
	public:
		static void Init();
		static auto SubmitTask(const std::function<void()>& task)
			-> std::shared_ptr<Task>;
		static void Shutdown();

		inline static auto GetTime() -> ThreadTime
		{
			return time;
		}

		[[nodiscard]] virtual inline auto GetID() const -> unsigned long
		{
			return std::hash<std::thread::id>()(thread.get_id());
		}

	private:
		inline static std::queue<std::shared_ptr<Task>> tasks;
		inline static std::thread thread;
		inline static std::mutex mutex;
		inline static ThreadTime time;
		inline static std::atomic<bool> running;
		inline static std::atomic<bool> finished;
		inline static std::condition_variable cv;

		static void Run();
	};
}