#pragma once
#include "utils/assets/Asset.h"

namespace Espresso::Assets
{
	class Texture : public Asset
	{
	public:
	private:
		int _width;
		int _height;
		int _comps;
	};
}