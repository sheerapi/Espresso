#include "core/EntryPoint.h"
#include "utils/threading/System.h"
#include "utils/threading/SystemManager.h"

using namespace Espresso;

class TestSystem : public Threading::System
{
	void Init() override
	{
		es_info("Hi");
	}

	void Shutdown() override
	{
		es_info("Bye");
	}
};

auto createApp(int argc, const char** argv) -> Application*
{
	auto* app = new Application("com.espresso.sandbox");

	Threading::SystemManager::AddSystem<TestSystem>();

	Threading::SystemManager::GetSystem(0)->Enqueue([]() { es_info("Enqueued"); });

	return app;
}