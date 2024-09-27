#pragma once
#include "Asset.h"
#include "core/Logger.h"
#include "utils/Demangle.h"
#include "utils/threading/Scheduler.h"
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
			Internals::typeCheck<Assets::Asset, T>();

			if (cache.contains(name))
			{
				return cache[name];
			}

			if (!registry.contains(name))
			{
				es_coreError("Asset {} not found in the registry!", name);
				return nullptr;
			}

			auto asset = std::make_shared<T>();
			cache[name] = asset;

			Threading::Scheduler::SubmitTask(
				[=]()
				{
					auto file = Read(registry[name]);
					asset->Load(file);
				});

			return asset;
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