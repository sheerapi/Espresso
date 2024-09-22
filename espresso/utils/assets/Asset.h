#pragma once

namespace Espresso::Assets
{
	class Asset
	{
	public:
		virtual void Load(char* data) {};
		virtual ~Asset() = default;
	};
}