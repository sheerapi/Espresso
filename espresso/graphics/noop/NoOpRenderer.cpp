#include "NoOpRenderer.h"
#include "SDL_error.h"
#include "SDL_video.h"
#include "core/Application.h"
#include "core/Logger.h"
#include "gl.h"

namespace Espresso::Graphics::NoOp
{
	void NoOpRenderer::Init()
	{
		ContextHandle = SDL_GL_CreateContext(
			(SDL_Window*)Application::main->GetWindow()->GetHandle());

		if (ContextHandle == nullptr)
		{
			es_coreError("Failed to create context, even for No-Op renderer...{}",
						 SDL_GetError());
			return;
		}

		gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress);
	}

	void NoOpRenderer::Swap()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow((SDL_Window*)Application::main->GetWindow()->GetHandle());
	}

	void NoOpRenderer::Shutdown()
	{
		SDL_GL_DeleteContext((SDL_GLContext)ContextHandle);
	}
}