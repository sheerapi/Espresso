#include "Math.h"
#include <random>

namespace Espresso
{
	auto Math::RandomFloat(float min, float max) -> float
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(min, max);
		return dist(gen);
	}

	auto Math::RandomInt(int min, int max) -> int
	{
		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(min, max);
		return dist(gen);
	}

	auto Math::PerlinNoise(float x) -> float
	{
		int X = (int)std::floor(x) & 255;
		x -= std::floor(x);
		float u = Fade(x);
		return Lerp(Grad(X, x), Grad(X + 1, x - 1), u);
	}

	auto Math::PerlinNoise(float x, float y) -> float
	{
		int X = (int)std::floor(x) & 255;
		int Y = (int)std::floor(y) & 255;
		x -= std::floor(x);
		y -= std::floor(y);
		float u = Fade(x);
		float v = Fade(y);
		int aa = (X + Y) & 255;
		int ab = (X + Y + 1) & 255;
		int ba = (X + 1 + Y) & 255;
		int bb = (X + 1 + Y + 1) & 255;

		return Lerp(Lerp(Grad(aa, x, y), Grad(ba, x - 1, y), u),
					Lerp(Grad(ab, x, y - 1), Grad(bb, x - 1, y - 1), u), v);
	}

	auto Math::Grad(int hash, float x, float y) -> float
	{
		int h = hash & 3;
		float u = h < 2 ? x : y;
		float v = h < 2 ? y : x;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

	auto Math::RandomString(int length) -> std::string
	{
		std::string result;

		for (int i = 0; i < length; i++)
		{
			result += (char)RandomInt(0x21, 0x7d);
		}

		return result;
	}
}