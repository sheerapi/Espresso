#pragma once
#include "Math.h"
#include "core/Logger.h"
#include "utils/math/Vector3.h"

namespace Espresso
{
	struct Vector4
	{
	public:
		inline static constexpr int Components = 4;

		float X;
		float Y;
		float Z;
		float W;

		Vector4() : X(0), Y(0), Z(0), W(0) {};

		Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {};

		Vector4(const Vector3& other) : X(other.X), Y(other.Y), Z(other.Z), W(0) {};

		Vector4(const Vector4& other) = default;

		inline static auto Cross(const Vector4& a, const Vector4& b) -> Vector4
		{
			return {(a.Y * b.Z) - (a.Z * b.Y), (a.Z * b.X) - (a.X * b.Z),
					(a.X * b.Y) - (a.Y * b.X), a.W * b.W};
		}

		auto operator=(const Vector4& other) -> Vector4&
		{
			if (this != &other)
			{
				X = other.X;
				Y = other.Y;
				Z = other.Z;
				W = other.W;
			}
			return *this;
		}

		auto operator+(const Vector4& other) const -> Vector4
		{
			return {X + other.X, Y + other.Y, Z + other.Z, W + other.W};
		}

		auto operator*(const Vector4& other) const -> Vector4
		{
			return {X * other.X, Y * other.Y, Z * other.Z, W * other.W};
		}

		auto operator-(const Vector4& other) const -> Vector4
		{
			return {X - other.X, Y - other.Y, Z - other.Z, W - other.W};
		}

		auto operator*(float scalar) const -> Vector4
		{
			return {X * scalar, Y * scalar, Z * scalar, W * scalar};
		}

		auto operator/(float scalar) const -> Vector4
		{
			if (scalar != 0)
			{
				return {X / scalar, Y / scalar, Z / scalar, W / scalar};
			}

			return *this;
		}

		[[nodiscard]] auto Dot(const Vector4& other) const -> float
		{
			return X * other.X + Y * other.Y + Z * other.Z + W * other.W;
		}

		[[nodiscard]] auto Magnitude() const -> float
		{
			return Math::SquareRoot(X * X + Y * Y + Z * Z + W * W);
		}

		[[nodiscard]] auto MagnitudeSquared() const -> float
		{
			return X * X + Y * Y + Z * Z + W * W;
		}

		[[nodiscard]] auto Normalized() const -> Vector4
		{
			float mag = Magnitude();
			if (mag != 0)
			{
				return {X / mag, Y / mag, Z / mag, W / mag};
			}

			return *this;
		}

		auto operator==(const Vector4& other) const -> bool
		{
			return (X == other.X && Y == other.Y && Z == other.Z && W == other.W);
		}

		auto operator<(const Vector4& other) const -> bool
		{
			return Magnitude() > other.Magnitude();
		}

		auto operator>(const Vector4& other) const -> bool
		{
			return Magnitude() < other.Magnitude();
		}

		auto operator<=(const Vector4& other) const -> bool
		{
			return Magnitude() >= other.Magnitude();
		}

		auto operator>=(const Vector4& other) const -> bool
		{
			return Magnitude() <= other.Magnitude();
		}

		auto operator!=(const Vector4& other) const -> bool
		{
			return !(*this == other);
		}

		auto operator*=(float scalar) -> Vector4&
		{
			X *= scalar;
			Y *= scalar;
			Z *= scalar;
			W *= scalar;
			return *this;
		}

		auto operator+=(const Vector4& other) -> Vector4&
		{
			X += other.X;
			Y += other.Y;
			Z += other.Z;
			W += other.W;
			return *this;
		}

		auto operator[](int index) -> float&
		{
			es_coreAssert(index < Components && index >= 0, "Out of bounds");

			switch (index)
			{
			case 0:
				return X;

			case 1:
				return Y;

			case 2:
				return Z;

			case 3:
				return W;

			default:
				return X;
			}
		}

		auto operator-=(const Vector4& other) -> Vector4&
		{
			X -= other.X;
			Y -= other.Y;
			Z -= other.Z;
			W -= other.W;
			return *this;
		}

		static auto Distance(const Vector4& v1, const Vector4& v2) -> float
		{
			return (v1 - v2).Magnitude();
		}

		[[nodiscard]] auto Reflect(const Vector4& normal) const -> Vector4
		{
			return *this - normal * 2.0F * Dot(normal);
		}

		inline static auto MoveTowards(const Vector4& current, const Vector4& target,
									   float maxDistanceDelta) -> Vector4
		{
			Vector4 direction = target - current;
			float distance = direction.Magnitude();

			if (distance <= maxDistanceDelta || distance == 0.0F)
			{
				return target;
			}

			return current + direction.Normalized() * maxDistanceDelta;
		}

		inline static auto Min(const Vector4& a, const Vector4& b) -> Vector4
		{
			return {Math::Min(a.X, b.X), Math::Min(a.Y, b.Y), Math::Min(a.Z, b.Z),
					Math::Min(a.W, b.W)};
		}

		inline static auto Max(const Vector4& a, const Vector4& b) -> Vector4
		{
			return {Math::Max(a.X, b.X), Math::Max(a.Y, b.Y), Math::Max(a.Z, b.Z),
					Math::Max(a.W, b.W)};
		}

		[[nodiscard]] inline auto Project(const Vector4& other) const -> Vector4
		{
			float dotProduct = this->Dot(other);
			float otherMagnitudeSquared = other.MagnitudeSquared();
			return other * (dotProduct / otherMagnitudeSquared);
		}

		inline static auto Angle(const Vector4& from, const Vector4& to) -> float
		{
			float dotProduct = from.Normalized().Dot(to.Normalized());
			return Math::Acos(Math::Clamp(dotProduct, -1.0F, 1.0F)) * (180.0F / Math::Pi);
		}

		[[nodiscard]] auto ClampMagnitude(float maxLength) const -> Vector4
		{
			float magnitude = Magnitude();
			if (magnitude > maxLength)
			{
				return *this * (maxLength / magnitude);
			}
			return *this;
		}

		static const Vector4 Zero;
		static const Vector4 One;
		static const Vector4 Up;
		static const Vector4 Down;
		static const Vector4 Left;
		static const Vector4 Right;
		static const Vector4 Back;
		static const Vector4 Forward;
	};

	inline const Vector4 Vector4::Zero(0.0F, 0.0F, 0.0F, 0.0F);
	inline const Vector4 Vector4::One(1.0F, 1.0F, 1.0F, 1.0F);
	inline const Vector4 Vector4::Up(0.0F, 1.0F, 0.F, 0.0F);
	inline const Vector4 Vector4::Down(0.0F, -1.0F, 0.0F, 0.0F);
	inline const Vector4 Vector4::Left(-1.0F, 0.0F, 0.0F, 0.0F);
	inline const Vector4 Vector4::Right(1.0F, 0.0F, 0.0F, 0.0F);
	inline const Vector4 Vector4::Back(0.0F, 0.0F, -1.0F, 0.0F);
	inline const Vector4 Vector4::Forward(0.0F, 0.0F, 1.0F, 0.0F);
}