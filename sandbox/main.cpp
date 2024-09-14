#include "core/Application.h"
#include "core/EntryPoint.h"
#include "utils/threading/System.h"
#include "utils/threading/SystemManager.h"

using namespace Espresso;

class TestSystem : public Threading::System
{
	void Tick() override
	{
	}
};

auto createApp(int argc, const char** argv) -> Application*
{
	auto* app = new Application("com.espresso.sandbox");
	app->CreateWindow("Sandbox");

	Threading::SystemManager::AddSystem<TestSystem>();

	return app;
}