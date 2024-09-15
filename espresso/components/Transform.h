#pragma once
#include "utils/math/Matrix4.h"
#include "utils/math/Quaternion.h"
#include "utils/math/Vector3.h"

namespace Espresso
{
	struct Transform
	{
	public:
		[[nodiscard]] auto HasChanged() const -> bool
		{
			return _hasChanged;
		}

		auto Position() -> Vector3
		{
			return _position;
		}

		auto Rotation() -> Quaternion
		{
			return _rotation;
		}

		auto Scale() -> Vector3
		{
			return _scale;
		}

	private:
		Vector3 _position;
		Quaternion _rotation;
		Vector3 _scale;
		Matrix4 _mvp;
		bool _hasChanged{true};

		inline void _tick() {};

		friend class Entity;
	};
}