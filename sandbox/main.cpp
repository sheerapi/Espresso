#include "components/Component.h"
#include "core/Application.h"
#include "core/EntryPoint.h"
#include "core/Scene.h"

using namespace Espresso;

class TestComponent : public Component
{
public:
	void Start() override
	{
		es_info("Hi");
	}
};

auto createApp(int argc, const char** argv) -> Application*
{
	auto* app = new Application("com.espresso.sandbox");
	app->CreateWindow("Sandbox");

	auto ent = Scene::main->AddEntity();
	ent->AddComponent<TestComponent>();

	ent->AddChild()->AddComponent<TestComponent>();

	return app;
}