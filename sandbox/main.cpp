#include "core/EntryPoint.h"
#include "core/EventManager.h"

auto createApp(int argc, const char** argv) -> Espresso::Application*
{
	auto* app = new Espresso::Application("com.espresso.sandbox");

	Espresso::EventManager::AddListener("present", [](auto data) { es_coreInfo("Hi"); });

	return app;
}