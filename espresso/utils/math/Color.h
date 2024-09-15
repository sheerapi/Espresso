#pragma once
#include "core/Logger.h"
#include "utils/math/Math.h"

namespace Espresso
{
	struct Color
	{
	public:
		inline static constexpr int Components = 4;

		int R;
		int G;
		int B;
		int A;

		Color() : R(255), G(255), B(255), A(255) {};
		Color(int red, int green, int blue, int alpha)
			: R(Math::Clamp(red, 0, 255)), G(Math::Clamp(green, 0, 255)),
			  B(Math::Clamp(blue, 0, 255)), A(Math::Clamp(alpha, 0, 255)) {};

		Color(int gray, int alpha)
			: R(Math::Clamp(gray, 0, 255)), G(Math::Clamp(gray, 0, 255)),
			  B(Math::Clamp(gray, 0, 255)), A(Math::Clamp(alpha, 0, 255))
		{
		}

		auto operator+(const Color& other) const -> Color
		{
			return {R + other.R, G + other.G, B + other.B, A + other.A};
		}

		auto operator-(const Color& other) const -> Color
		{
			return {R - other.R, G - other.G, B - other.B, A - other.A};
		}

		auto operator*(float scalar) const -> Color
		{
			return {
				static_cast<int>((float)R * scalar), static_cast<int>((float)G * scalar),
				static_cast<int>((float)B * scalar), static_cast<int>((float)A * scalar)};
		}

		auto operator*(const Color& other) const -> Color
		{
			return {R * other.R, G * other.G, B * other.B, A * other.A};
		}

		auto operator/(float scalar) const -> Color
		{
			if (scalar == 0)
			{
				return *this;
			}

			return {
				static_cast<int>((float)R / scalar), static_cast<int>((float)G / scalar),
				static_cast<int>((float)B / scalar), static_cast<int>((float)A / scalar)};
		}

		auto operator/(const Color& other) const -> Color
		{
			return {R / other.R, G / other.G, B / other.B, A / other.A};
		}

		auto operator==(const Color& other) const -> bool
		{
			return R == other.R && G == other.G && B == other.B && A == other.A;
		}

		auto operator!=(const Color& other) const -> bool
		{
			return !(*this == other);
		}

		auto operator[](int index) -> int&
		{
			es_coreAssert(index < Components && index > 0, "Out of bounds");

			switch (index)
			{
			case 0:
				return R;

			case 1:
				return G;

			case 2:
				return B;

			case 3:
				return A;

			default:
				return R;
			}
		}

		[[nodiscard]] auto ToGrayscale() const -> Color
		{
			int gray =
				static_cast<int>(0.21F * (float)R + 0.72F * (float)G + 0.07F * (float)B);
			return {gray, A};
		}

		[[nodiscard]] auto ApplyGamma(float gamma) const -> Color
		{
			auto applyGammaToChannel = [gamma](int c)
			{ return Math::Pow((float)c / 255.0F, gamma) * 255.0F; };
			return {static_cast<int>(applyGammaToChannel(R)),
					static_cast<int>(applyGammaToChannel(G)),
					static_cast<int>(applyGammaToChannel(B)), A};
		}

		[[nodiscard]] auto Invert() const -> Color
		{
			return {255 - R, 255 - G, 255 - B, A};
		}

		static auto Blend(const Color& c1, const Color& c2) -> Color
		{
			float alpha1 = (float)c1.A / 255.0F;
			float alpha2 = (float)c2.A / 255.0F;

			float rBlend = ((float)c1.R * alpha1) + ((float)c2.R * alpha2 * (1 - alpha1));
			float gBlend = ((float)c1.G * alpha1) + ((float)c2.G * alpha2 * (1 - alpha1));
			float bBlend = ((float)c1.B * alpha1) + ((float)c2.B * alpha2 * (1 - alpha1));
			float aBlend = (alpha1 + alpha2 * (1 - alpha1)) * 255.0F;

			return {static_cast<int>(rBlend), static_cast<int>(gBlend),
					static_cast<int>(bBlend), static_cast<int>(aBlend)};
		}
	};
}