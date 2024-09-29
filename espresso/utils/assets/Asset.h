#pragma once
#include <string>
#include <utility>

namespace Espresso::Assets
{
	class Asset
	{
	public:
		Asset(std::string name) : Name(std::move(name)) {};

		virtual void Load(const std::string& data) {};

		virtual ~Asset();

		[[nodiscard]] auto IsLoaded() const -> bool
		{
			return Loaded;
		}

	protected:
		bool Loaded;
		std::string Name;
	};
}