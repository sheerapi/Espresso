#include "core/EntryPoint.h"

auto createApp(int argc, const char** argv) -> Espresso::Application*
{
	auto* app = new Espresso::Application("com.espresso.sandbox");

	return app;
}