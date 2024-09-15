#include "System.h"
#include "core/Logger.h"
#include "utils/threading/SystemManager.h"
#include <mutex>
#include <thread>

namespace Espresso::Threading
{
	void System::Run()
	{
		try
		{
			std::unique_lock<std::mutex> lock(Mutex);
			SystemManager::GetCV().wait(lock, []() { return SystemManager::Ready(); });

			Init();

			while (SystemManager::Running())
			{
				Time.StartMeasure();
				ExecuteWorkQueue();
				Tick();
				Time.EndMeasure();
			}

			Shutdown();
			SystemManager::ReportShutdown();
			SystemManager::GetCV().notify_all();
		}
		catch (std::exception& e)
		{
			es_coreError("An error ocurred in {}: {}",
						 Internals::demangle(typeid(*this).name()), e.what());
			SystemManager::ReportShutdown();
			SystemManager::GetCV().notify_all();
		}
	}

	void System::ExecuteWorkQueue()
	{
		while (!_workQueue.empty())
		{
			_workQueue.back()();
			_workQueue.pop();
		}
	}

	void System::Initialize()
	{
		_thread = std::thread([=, this]() { Run(); });
		_thread.detach();
	}
}