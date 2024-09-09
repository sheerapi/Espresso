#pragma once
#include "Application.h"
#include "core/Logger.h"

extern auto createApp(int argc, const char** argv) -> Espresso::Application*;

auto main(int argc, const char** argv) -> int
{
	Espresso::Internals::initLogger();

	auto* app = createApp(argc, argv);

	auto success = app->Present();

	delete app;

	return static_cast<int>(success);
}