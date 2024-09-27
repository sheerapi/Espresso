#pragma once
#include "core/Logger.h"
#include <string>

namespace Espresso::Assets
{
	class Asset
	{
	public:
		virtual void Load(const std::string& data)
		{
			es_info(data);
		};

		virtual ~Asset() = default;

		[[nodiscard]] auto IsLoaded() const -> bool
		{
			return Loaded;
		}

	protected:
		bool Loaded;
	};
}