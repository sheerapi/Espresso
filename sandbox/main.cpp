#include "components/Component.h"
#include "core/Application.h"
#include "core/EntryPoint.h"
#include "core/Scene.h"
#include "utils/math/Matrix4.h"
#include "utils/math/Vector3.h"
#include "utils/math/Vector4.h"

using namespace Espresso;

class TestComponent : public Component
{
public:
	void Update() override
	{
	}

private:
	int _c{0};
	bool _t{false};
};

auto createApp(int argc, const char** argv) -> Application*
{
	auto* app = new Application("com.espresso.sandbox");
	app->CreateWindow("Sandbox");

	auto ent = Scene::main->AddEntity();
	ent->AddComponent<TestComponent>();

	return app;
}