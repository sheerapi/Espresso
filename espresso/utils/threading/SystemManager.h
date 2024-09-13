#pragma once
#include "utils/memory/Ref.h"
#include "utils/threading/System.h"
#include <vector>

namespace Espresso::Threading
{
	class SystemManager
	{
	public:
	private:
		std::vector<Ref<System>> _systems;
	};
}