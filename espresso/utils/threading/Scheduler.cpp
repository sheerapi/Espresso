#include "Scheduler.h"
#include "core/Logger.h"
#include <algorithm>
#include <mutex>
#include <thread>

namespace Espresso::Threading
{
	void Scheduler::Init()
	{
		running = true;
		thread = std::thread(Run);
		thread.detach();
	}

	void Scheduler::Shutdown()
	{
		running = false;

		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock, []() { return finished.load(); });
	}

	void Scheduler::Run()
	{
		while (running)
		{
			while (!tasks.empty())
			{
				(*tasks.begin())->_run();
				if ((*tasks.begin())->IsFinished())
				{
					tasks.erase(tasks.begin());
				}
			}

			CheckWaitingTasks();

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		finished = true;
		cv.notify_all();
	}

	void Scheduler::CheckWaitingTasks()
	{
		std::lock_guard<std::mutex> lock(mutex);
		for (auto it = waitingTasks.begin(); it != waitingTasks.end();)
		{
			if ((*it)->IsReady())
			{
				tasks.push_back(*it);
				std::sort(tasks.begin(), tasks.end(), [](auto a, auto b)
						  { return a->GetPriority() < b->GetPriority(); });
				it = waitingTasks.erase(it);
			}
			else
			{
				++it;
			}
		}
		cv.notify_one();
	}

	auto Scheduler::SubmitTask(const std::function<void()>& task,
							   TaskPriority priority) -> std::shared_ptr<Task>
	{
		return SubmitTask(std::make_shared<Task>(task, priority));
	}

	auto Scheduler::SubmitTask(const std::shared_ptr<Task>& task) -> std::shared_ptr<Task>
	{
		std::lock_guard<std::mutex> lock(mutex);

		if (tasks.size() >= 500)
		{
			es_coreWarn("Worker thread task soft limit exceeded, slow down!");
		}

		if (task->IsReady())
		{
			tasks.push_back(task);
			std::sort(tasks.begin(), tasks.end(),
					  [](auto a, auto b) { return a->GetPriority() < b->GetPriority(); });
		}
		else
		{
			waitingTasks.push_back(task);
			std::sort(waitingTasks.begin(), waitingTasks.end(),
					  [](auto a, auto b) { return a->GetPriority() < b->GetPriority(); });
		}
		cv.notify_one();
		return task;
	}
}