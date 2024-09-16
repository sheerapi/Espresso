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

		auto Position() -> Vector3;
		auto Rotation() -> Quaternion;
		auto Scale() -> Vector3;

		void Position(Vector3 newPos);
		void Rotation(Quaternion newRot);
		void Scale(Vector3 newScale);

		[[nodiscard]] auto Forward() -> Vector3;
		[[nodiscard]] auto Up() -> Vector3;
		[[nodiscard]] auto Right() -> Vector3;

		auto GetTransformationMatrix() -> Matrix4;

	private:
		Vector3 _position;
		Quaternion _rotation;
		Vector3 _scale{Vector3::One};
		Matrix4 _mvp;
		class Entity* _entity;
		bool _hasChanged{true};

		void _tick();

		friend class Entity;
	};
}