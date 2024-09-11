#pragma once
#include "ThreadTime.h"
#include <atomic>
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

	protected:
		virtual void Run();
		ThreadTime Time;
		std::thread Thread;
	};
}