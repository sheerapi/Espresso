#include "core/Application.h"
#include "core/EntryPoint.h"
#include "utils/math/Vector2.h"

using namespace Espresso;

auto createApp(int argc, const char** argv) -> Application*
{
	auto* app = new Application("com.espresso.sandbox");
	app->CreateWindow("Sandbox");

	es_coreInfo("{}", app->GetWindow()->ToString());

	return app;
}