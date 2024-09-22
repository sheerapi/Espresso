#pragma once
#include <memory>

namespace Espresso::Assets
{
	class AssetManager
	{
	public:
		template <typename T> auto Load(const std::string& name) -> std::shared_ptr<T>
		{
		}
	};
}