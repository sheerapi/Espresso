#include "Renderer.h"
#include "core/Application.h"
#include "core/Logger.h"
#include "graphics/noop/NoOpRenderer.h"
#include "utils/Demangle.h"

namespace Espresso::Graphics
{
	void Renderer::ChooseBackend()
	{
		if (main != nullptr)
		{
			return;
		}

		auto platform = Espresso::Application::QueryPlatform();

		main = new NoOp::NoOpRenderer();

		es_coreInfo("Initialized new {} renderer!",
					Internals::demangle(typeid(*main).name()));
	}
}