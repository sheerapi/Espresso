#pragma once
#include "glm/glm.hpp" // IWYU pragma: keep
#include <cmath>
#include <string>

namespace Espresso
{
	class Math
	{
	public:
		inline static constexpr float Pi = 3.14159265358979323846F;
		inline static constexpr float Tau = 2.F * Pi;
		inline static constexpr float HalfPi = 0.5F * Pi;
		inline static constexpr float E = 2.7182818284590451F;

		inline static auto DegreesToRadians(float degrees) -> float
		{
			return degrees * (Pi / 180.0F);
		}

		inline static auto RadiansToDegrees(float radians) -> float
		{
			return radians * (180.0F / Pi);
		}

		inline static auto SquareRoot(float val) -> float
		{
			return glm::sqrt(val);
		}

		inline static auto Abs(float val) -> float
		{
			return std::fabs(val);
		}

		inline static auto Sin(float val) -> float
		{
			return glm::sin(val);
		}

		inline static auto Cos(float val) -> float
		{
			return glm::cos(val);
		}

		inline static auto Tan(float val) -> float
		{
			return glm::tan(val);
		}

		inline static auto Asin(float val) -> float
		{
			return glm::asin(val);
		}

		inline static auto Acos(float val) -> float
		{
			return glm::acos(val);
		}

		inline static auto Atan(float val) -> float
		{
			return glm::atan(val);
		}

		inline static auto Atan2(float x, float y) -> float
		{
			return std::atan2(x, y);
		}

		inline static auto Clamp(float value, float min, float max) -> float
		{
			if (value < min)
			{
				return min;
			}
			if (value > max)
			{
				return max;
			}
			return value;
		}

		template <typename T> static auto Clamp(T value, T min, T max) -> T
		{
			if (value < min)
			{
				return min;
			}
			if (value > max)
			{
				return max;
			}
			return value;
		}

		inline static auto Lerp(float a, float b, float t) -> float
		{
			return a + t * (b - a);
		}

		template <typename T> static auto Lerp(T a, T b, T t) -> float
		{
			return a + t * (b - a);
		}

		static auto RandomFloat(float min = 0.0F, float max = 1.0F) -> float;

		static auto RandomInt(int min = 0, int max = 1) -> int;

		inline static auto RandomBool() -> bool
		{
			return RandomInt() != 0;
		}

		static auto RandomString(int length) -> std::string;

		static auto PerlinNoise(float x) -> float;

		static auto PerlinNoise(float x, float y) -> float;

	private:
		static auto Fade(float t) -> float
		{
			return t * t * t * (t * (t * 6.0F - 15.0F) + 10.0F);
		}

		static auto Grad(int hash, float x) -> float
		{
			return (hash & 1) == 0 ? x : -x;
		}

		static auto Grad(int hash, float x, float y) -> float;
	};
}