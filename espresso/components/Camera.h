#pragma once
#include "components/Component.h"
#include "components/Transform.h"
#include "utils/math/Color.h"
#include "utils/math/Matrix4.h"

namespace Espresso::Components
{
	enum class CameraType
	{
		Perspective,
		Orthographic
	};

	class Camera : public Component
	{
	public:
		inline static Camera* main{nullptr};

		inline Camera()
		{
			if (main == nullptr)
			{
				main = this;
			}
		}

		CameraType Type;
		Color BackgroundColor{Color(0, 0, 0, 255)};
		float FOV{60};
		float Near{0.01F};
		float Far{100};

		inline auto GetViewMatrix() -> Matrix4
		{
			return Matrix4::LookAt(Transform->Position(), Transform->Forward(),
								   Transform->Up());
		}

		auto GetProjectionMatrix() -> Matrix4;
	};
}