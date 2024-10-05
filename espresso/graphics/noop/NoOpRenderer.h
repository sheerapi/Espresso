#pragma once
#include "graphics/Renderer.h"

namespace Espresso::Graphics::NoOp
{
	class NoOpRenderer : public Renderer
	{
	public:
		inline auto GetInternalBackendFlag() -> unsigned long override
		{
			return 0;
		}
	};
}