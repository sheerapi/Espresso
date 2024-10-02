#pragma once
#include "Asset.h"
#include "core/Logger.h"
#include "utils/Demangle.h"
#include "utils/threading/Scheduler.h"
#include <memory>
#include <unordered_map>

#ifdef DEBUG
#	include "zstd.h"
#endif

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
				return cache[name].lock();
			}

			if (!registry.contains(name))
			{
				es_coreError("Asset {} not found in the registry!", name);
				return nullptr;
			}

			auto asset = std::make_shared<T>(name);
			cache[name] = asset;

			Threading::Scheduler::SubmitTask(
				[=]()
				{
					auto file = Read(registry[name]);
					asset->Load(file);
				});

			return asset;
		}

		inline static void UnloadAsset(const std::string& name)
		{
			if (!cache.contains(name))
			{
				es_coreError("Asset {} is not loaded!", name);
				return;
			}

			cache.erase(cache.find(name));
		}

	private:
		inline static std::unordered_map<std::string, std::weak_ptr<Assets::Asset>> cache;
		inline static std::unordered_map<std::string, std::string> registry;

#ifdef DEBUG
		inline static std::unordered_map<std::string, unsigned int> registryOffsets;
		inline static ZSTD_DCtx* dctx;
#endif

		static void Init();
		static auto Read(const std::string& path) -> std::string;

		friend class Application;
	};
}