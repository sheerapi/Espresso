#include "Asset.h"
#include "utils/assets/AssetManager.h"

namespace Espresso::Assets
{
	Asset::~Asset()
	{
		AssetManager::UnloadAsset(Name);
	}
}