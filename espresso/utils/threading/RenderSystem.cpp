#include "RenderSystem.h"
#include "core/Application.h"
#include "graphics/Renderer.h"

namespace Espresso::Threading
{
	void RenderSystem::Init()
	{
		if (Application::main->GetWindow() != nullptr)
		{
			Graphics::Renderer::main->Init();
		}
	}

	void RenderSystem::Tick()
	{
		if (Application::main->GetWindow() != nullptr)
		{
			Graphics::Renderer::main->Swap();
		}
	}

	void RenderSystem::Shutdown()
	{
		if (Application::main->GetWindow() != nullptr)
		{
			Graphics::Renderer::main->Shutdown();
		}
	}
}