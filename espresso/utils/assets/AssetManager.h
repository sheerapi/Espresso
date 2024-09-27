#pragma once
#include "Asset.h"
#include <memory>
#include <unordered_map>

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
		inline static std::unordered_map<std::string, std::shared_ptr<Assets::Asset>>
			cache;
		inline static std::unordered_map<std::string, std::string> registry;

		static void Init();
		static auto Read(const std::string& path) -> std::string;

		friend class Application;
	};
}