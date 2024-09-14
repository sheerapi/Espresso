#pragma once
#include "Math.h"
#include "core/Object.h"

namespace Espresso
{
	struct Vector3 : public Object
	{
	public:
		float X;
		float Y;
		float Z;

		Vector3() : X(0), Y(0), Z(0)
		{
		}

		Vector3(float x, float y, float z) : X(x), Y(y), Z(z)
		{
		}

		Vector3(const Vector3& other) = default;

		auto operator=(const Vector3& other) -> Vector3&
		{
			if (this != &other)
			{
				X = other.X;
				Y = other.Y;
				Z = other.Z;
			}
			return *this;
		}

		auto operator+(const Vector3& other) const -> Vector3
		{
			return {X + other.X, Y + other.Y, Z + other.Z};
		}

		auto operator-(const Vector3& other) const -> Vector3
		{
			return {X - other.X, Y - other.Y, Z - other.Z};
		}

		auto operator*(float scalar) const -> Vector3
		{
			return {X * scalar, Y * scalar, Z * scalar};
		}

		auto operator/(float scalar) const -> Vector3
		{
			if (scalar != 0)
			{
				return {X / scalar, Y / scalar, Z / scalar};
			}

			return *this;
		}

		inline static auto Cross(const Vector3& a, const Vector3& b) -> Vector3
		{
			return {(a.Y * b.Z) - (a.Z * b.Y), (a.Z * b.X) - (a.X * b.Z),
					(a.X * b.Y) - (a.Y * b.X)};
		}

		[[nodiscard]] auto Dot(const Vector3& other) const -> float
		{
			return X * other.X + Y * other.Y + Z * other.Z;
		}

		[[nodiscard]] auto Magnitude() const -> float
		{
			return Math::SquareRoot(X * X + Y * Y + Z * Z);
		}

		[[nodiscard]] auto MagnitudeSquared() const -> float
		{
			return X * X + Y * Y + Z * Z;
		}

		[[nodiscard]] auto Normalized() const -> Vector3
		{
			float mag = Magnitude();
			if (mag != 0)
			{
				return {X / mag, Y / mag, Z / mag};
			}

			return *this;
		}

		auto operator==(const Vector3& other) const -> bool
		{
			return (X == other.X && Y == other.Y && Z == other.Z);
		}

		auto operator<(const Vector3& other) const -> bool
		{
			return Magnitude() > other.Magnitude();
		}

		auto operator>(const Vector3& other) const -> bool
		{
			return Magnitude() < other.Magnitude();
		}

		auto operator<=(const Vector3& other) const -> bool
		{
			return Magnitude() >= other.Magnitude();
		}

		auto operator>=(const Vector3& other) const -> bool
		{
			return Magnitude() <= other.Magnitude();
		}

		auto operator!=(const Vector3& other) const -> bool
		{
			return !(*this == other);
		}

		auto operator*=(float scalar) -> Vector3&
		{
			X *= scalar;
			Y *= scalar;
			Z *= scalar;
			return *this;
		}

		auto operator+=(const Vector3& other) -> Vector3&
		{
			X += other.X;
			Y += other.Y;
			Z += other.Z;
			return *this;
		}

		auto operator-=(const Vector3& other) -> Vector3&
		{
			X -= other.X;
			Y -= other.Y;
			Z -= other.Z;
			return *this;
		}

		static auto Distance(const Vector3& v1, const Vector3& v2) -> float
		{
			return (v1 - v2).Magnitude();
		}

		[[nodiscard]] auto ToString() const -> std::string override
		{
			return std::format("[{} .X = {}, .Y = {}, .Z = {} ({})]", GetName(), X, Y, Z,
							   static_cast<const void*>(this));
		}

		[[nodiscard]] auto Reflect(const Vector3& normal) const -> Vector3
		{
			return *this - normal * 2.0F * Dot(normal);
		}

		inline static auto MoveTowards(const Vector3& current, const Vector3& target,
									   float maxDistanceDelta) -> Vector3
		{
			Vector3 direction = target - current;
			float distance = direction.Magnitude();

			if (distance <= maxDistanceDelta || distance == 0.0F)
			{
				return target;
			}

			return current + direction.Normalized() * maxDistanceDelta;
		}

		inline static auto Min(const Vector3& a, const Vector3& b) -> Vector3
		{
			return {Math::Min(a.X, b.X), Math::Min(a.Y, b.Y), Math::Min(a.Z, b.Z)};
		}

		inline static auto Max(const Vector3& a, const Vector3& b) -> Vector3
		{
			return {Math::Max(a.X, b.X), Math::Max(a.Y, b.Y), Math::Max(a.Z, b.Z)};
		}

		[[nodiscard]] inline auto Project(const Vector3& other) const -> Vector3
		{
			float dotProduct = this->Dot(other);
			float otherMagnitudeSquared = other.MagnitudeSquared();
			return other * (dotProduct / otherMagnitudeSquared);
		}

		inline static auto Angle(const Vector3& from, const Vector3& to) -> float
		{
			float dotProduct = from.Normalized().Dot(to.Normalized());
			return Math::Acos(Math::Clamp(dotProduct, -1.0F, 1.0F)) * (180.0F / Math::Pi);
		}

		[[nodiscard]] auto ClampMagnitude(float maxLength) const -> Vector3
		{
			float magnitude = Magnitude();
			if (magnitude > maxLength)
			{
				return *this * (maxLength / magnitude);
			}
			return *this;
		}

		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Left;
		static const Vector3 Right;
		static const Vector3 Back;
		static const Vector3 Forward;
	};

	const Vector3 Vector3::Zero(0.0F, 0.0F, 0.0F);
	const Vector3 Vector3::One(1.0F, 1.0F, 1.0F);
	const Vector3 Vector3::Up(0.0F, 1.0F, 0.F);
	const Vector3 Vector3::Down(0.0F, -1.0F, 0.0F);
	const Vector3 Vector3::Left(-1.0F, 0.0F, 0.0F);
	const Vector3 Vector3::Right(1.0F, 0.0F, 0.0F);
	const Vector3 Vector3::Back(0.0F, 0.0F, -1.0F);
	const Vector3 Vector3::Forward(1.0F, 0.0F, 1.0F);
}