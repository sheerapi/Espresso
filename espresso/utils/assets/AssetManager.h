#pragma once
#include <memory>

namespace Espresso
{
	class AssetManager
	{
	public:
		template <typename T>
		static auto Load(const std::string& name) -> std::shared_ptr<T>
		{
		}

	private:
		static void Init();

		friend class Application;
	};
}