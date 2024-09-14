#include "core/Application.h"
#include "core/EntryPoint.h"
#include "utils/math/Math.h"

using namespace Espresso;

auto createApp(int argc, const char** argv) -> Application*
{
	auto* app = new Application("com.espresso.sandbox");
	app->CreateWindow("Sandbox");

	es_coreInfo("{}", Math::RandomString(24));

	return app;
}