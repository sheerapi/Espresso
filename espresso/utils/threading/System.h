#pragma once
#include <string>

namespace Espresso::Threading
{
	class System
	{
	public:
		virtual void Init() {};
		virtual void Shutdown() {};
		virtual void Tick() = 0;

		[[nodiscard]] virtual inline auto GetName() const -> std::string
		{
			return "System";
		}

		[[nodiscard]] virtual inline auto RunSeparate() const -> bool
		{
			return true;
		}

	protected:
		virtual void Run();
	};
}