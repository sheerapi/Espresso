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
	};
}