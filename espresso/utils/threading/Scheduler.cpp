#include "Scheduler.h"
#include "core/Logger.h"
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
				tasks.front()->_run();
				tasks.pop();
				es_coreDebug("Executed scheduled task, idling...");
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		finished = true;
		cv.notify_all();
	}

	auto Scheduler::SubmitTask(const std::function<void()>& task) -> std::shared_ptr<Task>
	{
		std::lock_guard<std::mutex> lock(mutex);

		if (tasks.size() >= 500)
		{
			es_coreWarn("Worked thread task soft limit exceeded, slow down!");
		}

		auto t = std::make_shared<Task>(task);
		tasks.push(t);
		cv.notify_one();
		return t;
	}
}