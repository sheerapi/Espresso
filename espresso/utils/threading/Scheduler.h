#pragma once
#include "core/Logger.h"
#include "utils/threading/ThreadTime.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace Espresso::Threading
{
	enum class TaskPriority
	{
		High,
		Normal,
		Low
	};

	class Task
	{
	public:
		Task(const std::function<void()>& task,
			 TaskPriority priority = TaskPriority::Normal)
			: _task(task), _priority(priority) {};

		inline void OnFinish(const std::function<void()>& callback)
		{
			_callback = callback;
		}

		inline void AddDependency(const std::shared_ptr<Task>& task)
		{
			_dependencies++;
			task->OnFinish([this]() { _dependencies--; });
		}

		inline auto IsFinished() -> bool
		{
			return _finished;
		}

		inline auto IsReady() -> bool
		{
			return _dependencies == 0;
		}

		inline auto GetPriority() -> TaskPriority
		{
			return _priority;
		}

	private:
		std::function<void()> _task;
		std::function<void()> _callback;
		std::atomic<bool> _finished{false};
		std::atomic<int> _dependencies{0};
		TaskPriority _priority;

		inline void _run()
		{
			if (!IsReady())
			{
				return;
			}

			_task();
			_finished = true;

			if (_callback)
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
		static auto SubmitTask(const std::function<void()>& task,
							   TaskPriority priority = TaskPriority::Normal)
			-> std::shared_ptr<Task>;
		static auto SubmitTask(const std::shared_ptr<Task>& task)
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
		inline static std::vector<std::shared_ptr<Task>> tasks;
		inline static std::vector<std::shared_ptr<Task>> waitingTasks;
		inline static std::thread thread;
		inline static std::mutex mutex;
		inline static ThreadTime time;
		inline static std::atomic<bool> running;
		inline static std::atomic<bool> finished;
		inline static std::condition_variable cv;

		static void Run();
		static void CheckWaitingTasks();
	};
}