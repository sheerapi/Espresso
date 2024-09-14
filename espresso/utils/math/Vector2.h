#pragma once
#include "Math.h"
#include "core/Object.h"

namespace Espresso
{
	struct Vector2 : public Object
	{
	public:
		float X;
		float Y;

		Vector2() : X(0), Y(0)
		{
		}

		Vector2(float x, float y) : X(x), Y(y)
		{
		}

		Vector2(const Vector2& other) = default;

		auto operator=(const Vector2& other) -> Vector2&
		{
			if (this != &other)
			{
				X = other.X;
				Y = other.Y;
			}
			return *this;
		}

		auto operator+(const Vector2& other) const -> Vector2
		{
			return {X + other.X, Y + other.Y};
		}

		auto operator-(const Vector2& other) const -> Vector2
		{
			return {X - other.X, Y - other.Y};
		}

		auto operator*(float scalar) const -> Vector2
		{
			return {X * scalar, Y * scalar};
		}

		auto operator/(float scalar) const -> Vector2
		{
			if (scalar != 0)
			{
				return {X / scalar, Y / scalar};
			}

			return *this;
		}

		[[nodiscard]] auto Dot(const Vector2& other) const -> float
		{
			return X * other.X + Y * other.Y;
		}

		[[nodiscard]] auto Magnitude() const -> float
		{
			return Math::SquareRoot(X * X + Y * Y);
		}

		[[nodiscard]] auto MagnitudeSquared() const -> float
		{
			return X * X + Y * Y;
		}

		[[nodiscard]] auto Normalized() const -> Vector2
		{
			float mag = Magnitude();
			if (mag != 0)
			{
				return {X / mag, Y / mag};
			}

			return *this;
		}

		auto operator==(const Vector2& other) const -> bool
		{
			return (X == other.X && Y == other.Y);
		}

		auto operator<(const Vector2& other) const -> bool
		{
			return Magnitude() > other.Magnitude();
		}

		auto operator>(const Vector2& other) const -> bool
		{
			return Magnitude() < other.Magnitude();
		}

		auto operator<=(const Vector2& other) const -> bool
		{
			return Magnitude() >= other.Magnitude();
		}

		auto operator>=(const Vector2& other) const -> bool
		{
			return Magnitude() <= other.Magnitude();
		}

		auto operator!=(const Vector2& other) const -> bool
		{
			return !(*this == other);
		}

		auto operator*=(float scalar) -> Vector2&
		{
			X *= scalar;
			Y *= scalar;
			return *this;
		}

		auto operator+=(const Vector2& other) -> Vector2&
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		auto operator-=(const Vector2& other) -> Vector2&
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		static auto Distance(const Vector2& v1, const Vector2& v2) -> float
		{
			return (v1 - v2).Magnitude();
		}

		[[nodiscard]] auto ToString() const -> std::string override
		{
			return std::format("[{} .X = {}, .Y = {} ({})]", GetName(), X, Y,
							   static_cast<const void*>(this));
		}

		[[nodiscard]] auto Reflect(const Vector2& normal) const -> Vector2
		{
			return *this - normal * 2.0F * Dot(normal);
		}

		[[nodiscard]] auto Perpendicular() const -> Vector2
		{
			return {-Y, X};
		}

		inline static auto MoveTowards(const Vector2& current, const Vector2& target,
									   float maxDistanceDelta) -> Vector2
		{
			Vector2 direction = target - current;
			float distance = direction.Magnitude();

			if (distance <= maxDistanceDelta || distance == 0.0F)
			{
				return target;
			}

			return current + direction.Normalized() * maxDistanceDelta;
		}

		inline static auto Min(const Vector2& a, const Vector2& b) -> Vector2
		{
			return {Math::Min(a.X, b.X), Math::Min(a.Y, b.Y)};
		}

		inline static auto Max(const Vector2& a, const Vector2& b) -> Vector2
		{
			return {Math::Max(a.X, b.X), Math::Max(a.Y, b.Y)};
		}

		[[nodiscard]] inline auto Project(const Vector2& other) const -> Vector2
		{
			float dotProduct = this->Dot(other);
			float otherMagnitudeSquared = other.MagnitudeSquared();
			return other * (dotProduct / otherMagnitudeSquared);
		}

		inline static auto Angle(const Vector2& from, const Vector2& to) -> float
		{
			float dotProduct = from.Normalized().Dot(to.Normalized());
			return Math::Acos(Math::Clamp(dotProduct, -1.0F, 1.0F)) * (180.0F / Math::Pi);
		}

		[[nodiscard]] auto ClampMagnitude(float maxLength) const -> Vector2
		{
			float magnitude = Magnitude();
			if (magnitude > maxLength)
			{
				return *this * (maxLength / magnitude);
			}
			return *this;
		}

		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 Up;
		static const Vector2 Down;
		static const Vector2 Left;
		static const Vector2 Right;
	};

	const Vector2 Vector2::Zero(0.0F, 0.0F);
	const Vector2 Vector2::One(1.0F, 1.0F);
	const Vector2 Vector2::Up(0.0F, 1.0F);
	const Vector2 Vector2::Down(0.0F, -1.0F);
	const Vector2 Vector2::Left(-1.0F, 0.0F);
	const Vector2 Vector2::Right(1.0F, 0.0F);
}