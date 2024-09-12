#include "System.h"
#include "core/Logger.h"
#include <thread>

namespace Espresso::Threading
{
	void System::Run()
	{
		try
		{
			Init();
			Running.store(true);

			while (Running.load())
			{
				Time.StartMeasure();
				ExecuteWorkQueue();
				Tick();
				Time.EndMeasure();
			}

			Running.store(false);

			Shutdown();
		}
		catch (std::exception& e)
		{
			es_coreError("An error ocurred in {}: {}", GetName(), e.what());
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
		_thread = std::thread(
			[=, this]()
			{
				_id = std::this_thread::get_id();
				Run();
			});
	}
}