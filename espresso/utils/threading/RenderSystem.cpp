#include "RenderSystem.h"
#include "graphics/Renderer.h"

namespace Espresso::Threading
{
	void RenderSystem::Init()
	{
		Graphics::Renderer::main->Init();
	}

	void RenderSystem::Tick()
	{
		Graphics::Renderer::main->Swap();
	}

	void RenderSystem::Shutdown()
	{
		Graphics::Renderer::main->Shutdown();
	}
}