#include "components/Component.h"
#include "core/Application.h"
#include "core/EntryPoint.h"
#include "core/Scene.h"
#include "utils/assets/AssetManager.h"

using namespace Espresso;

class TestComponent : public Component
{
public:
	void Start() override
	{
		auto asset = AssetManager::Load<Assets::Asset>("default");
	}
};

auto createApp(int argc, const char** argv) -> Application*
{
	auto* app = new Application();
	app->CreateWindow("Sandbox", 100, 100);

	auto ent = Scene::main->AddEntity();
	ent->AddComponent<TestComponent>();

	return app;
}