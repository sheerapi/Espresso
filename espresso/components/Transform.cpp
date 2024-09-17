#include "Transform.h"
#include "components/Camera.h"
#include "core/Entity.h"
#include "utils/math/Matrix4.h"
#include "utils/math/Vector3.h"

namespace Espresso
{
	auto Transform::Position() -> Vector3
	{
		return _position + (_entity->Parent() != nullptr
								? _entity->Parent()->Transform.Position()
								: Vector3::Zero);
	}

	auto Transform::Rotation() -> Quaternion
	{
		return _rotation + (_entity->Parent() != nullptr
								? _entity->Parent()->Transform.Rotation()
								: Quaternion());
	}

	auto Transform::Scale() -> Vector3
	{
		return _scale * (_entity->Parent() != nullptr
							 ? _entity->Parent()->Transform.Scale()
							 : Vector3::One);
	}

	void Transform::Position(Vector3 newPos)
	{
		_position = newPos;
		_entity->NotifyChange();
	}

	void Transform::Rotation(Quaternion newRot)
	{
		_rotation = newRot;
		_entity->NotifyChange();
	}

	void Transform::Scale(Vector3 newScale)
	{
		_scale = newScale;
		_entity->NotifyChange();
	}

	auto Transform::Forward() -> Vector3
	{
		return Rotation() * Vector3(0.0F, 0.0F, 1.0F);
	}

	auto Transform::Up() -> Vector3
	{
		return Rotation() * Vector3(0.0F, 1.0F, 0.0F);
	}

	auto Transform::Right() -> Vector3
	{
		return Rotation() * Vector3(1.0F, 0.0F, 0.0F);
	}

	auto Transform::GetTransformationMatrix() -> Matrix4
	{
		return _mvp;
	}

	void Transform::_tick()
	{
		if (_hasChanged)
		{
			return;
		}

		Matrix4 model = Matrix4::Translate(Vector4(Position())) *
						Matrix4::ToMatrix(Rotation()) * Matrix4::Scale(Vector4(Scale()));

		if (Components::Camera::main != nullptr)
		{
			_mvp = Components::Camera::main->GetProjectionMatrix() *
				   Components::Camera::main->GetViewMatrix() * model;
		}
	}
}