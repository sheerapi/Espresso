#include "components/Camera.h"
#include "components/Component.h"
#include "core/Application.h"
#include "core/EntryPoint.h"
#include "core/Scene.h"
#include "utils/math/Matrix4.h"
#include "utils/math/Quaternion.h"
#include "utils/math/Vector3.h"
#include "utils/math/Vector4.h"

using namespace Espresso;

class TestComponent : public Component
{
public:
	void Start() override
	{
		auto m = Matrix4::Scale(Vector4(Vector3::One)) * Matrix4::ToMatrix(Quaternion()) *
				 Matrix4::Translate(Vector4(Vector3::Zero));
		es_info("{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}", m.Row0.X, m.Row0.Y,
				m.Row0.Z, m.Row0.W, m.Row1.X, m.Row1.Y, m.Row1.Z, m.Row1.W, m.Row2.X,
				m.Row2.Y, m.Row2.Z, m.Row2.W, m.Row3.X, m.Row3.Y, m.Row3.Z, m.Row3.W);
	}

	void Update() override
	{
		if (_c < 1000)
		{
			_c++;
			return;
		}

		if (!_t)
		{
			auto m = Transform->GetTransformationMatrix();
			es_info("{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}", m.Row0.X,
					m.Row0.Y, m.Row0.Z, m.Row0.W, m.Row1.X, m.Row1.Y, m.Row1.Z, m.Row1.W,
					m.Row2.X, m.Row2.Y, m.Row2.Z, m.Row2.W, m.Row3.X, m.Row3.Y, m.Row3.Z,
					m.Row3.W);

			_t = true;
		}
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