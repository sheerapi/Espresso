#pragma once
#include "graphics/Renderer.h"

namespace Espresso::Graphics::NoOp
{
	class NoOpRenderer : public Renderer
	{
	public:
		inline auto GetInternalBackendFlag() -> unsigned long override
		{
			// SDL_WINDOW_OPENGL
			return 2;
		}

		void Init() override;
		void Shutdown() override;
		void Swap() override;
	};
}