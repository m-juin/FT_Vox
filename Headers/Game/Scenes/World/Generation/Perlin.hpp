#ifndef __PERLIN_HPP__
#define __PERLIN_HPP__
#include <cstdint>

#include "MathGraphicalLib/Vectors/Vector2.hpp"

#include "./PerlinUtils.hpp"
#include "Spline/Spline.hpp"

#include "Game/Scenes/World/Utils/Defines.hpp"

#include <unordered_map>

namespace Vox::Game::Generation::Perlins
{
	namespace
	{
		constexpr uint32_t WORLD_CENTER = 125000;
		static inline MGL::Vectors::Vector2<float> randomGradient(int ix, int iy, uint32_t seed)
		{
			const unsigned w = 8 * sizeof(unsigned);
			const unsigned s = w / 2;
			unsigned a = ix, b = iy;
			a *= seed;

			b ^= a << s | a >> (w - s);
			b *= 1911520717;

			a ^= b << s | b >> (w - s);
			a *= 2048419325;
			float random = a * (3.14159265 / ~(~0u >> 1));

			MGL::Vectors::Vector2<float> v;
			v[0] = sin(random);
			v[1] = cos(random);

			return v;
		}

		static inline float dotGridGradient(int ix, int iy, float x, float y, uint32_t seed)
		{
			MGL::Vectors::Vector2<float> gradient = randomGradient(ix, iy, seed);

			float dx = x - (float)ix;
			float dy = y - (float)iy;

			return (dx * gradient[0] + dy * gradient[1]);
		}

		static inline float interpolate(float a0, float a1, float w)
		{
			return (a1 - a0) * (w * w * w * (w * (w * 6 - 15) + 10)) + a0;
		}

		static inline float perlin(float x, float y, uint32_t seed)
		{

			int x0 = (long int)x;
			int y0 = (long int)y;
			int x1 = x0 + 1;
			int y1 = y0 + 1;

			float sx = x - (float)x0;
			float sy = y - (float)y0;

			float n0 = dotGridGradient(x0, y0, x, y, seed);
			float n1 = dotGridGradient(x1, y0, x, y, seed);
			float ix0 = interpolate(n0, n1, sx);

			n0 = dotGridGradient(x0, y1, x, y, seed);
			n1 = dotGridGradient(x1, y1, x, y, seed);
			float ix1 = interpolate(n0, n1, sx);

			float value = interpolate(ix0, ix1, sy);

			return value;
		}
	} // namespace
	inline double GetPerlinValue(float x, float y, uint32_t seed, Utils::PerlinData data,
								 std::pair<double, double> range)
	{
		float val = 0.0f;

		float amp = data.amp;
		float freq = data.freq;
		float maxAmp = 0.0f;

		for (int counter = 0; counter < data.octaves; counter++)
		{
			val += amp * (perlin((x + data.offset[0]) * freq, (y + data.offset[1]) * freq, seed));
			freq *= 2;
			maxAmp += amp;
			amp /= 2;
		}
		// val = val / maxAmp;
		// val = valueRange.x + (val - -1.0f) * (valueRange.y - valueRange.x) / (1.0f - -1.0f);
		// val = ((val -1.0f) / (1.0f - 1.0f)) * (valueRange.y - valueRange.x) * valueRange.x;
		return Spline::GetNormalizedRangedValue(val, {-1.0f, 1.0f}, range);
	}
	inline uint8_t GetHeightAt(int x, int z, uint32_t seed,
							   const std::unordered_map<std::string, const Spline::Spline> &spl)
	{
		// applique exactement le même perlin et spline que dans GenerateHeightMap
		float val = spl.at("Continental")
						.GetValue(GetPerlinValue(x + WORLD_CENTER, z + WORLD_CENTER, seed, Utils::ContinentalnessData,
												 {-1.2f, 1.0f}));

		return static_cast<uint8_t>(val);
	}
	inline bool IsBlockAt(const Game::Utils::Defines::Vector3Int pos, const uint32_t &seed,
						  const std::unordered_map<std::string, const Spline::Spline> &spl)
	{
		uint8_t h = GetHeightAt(pos[0], pos[2], seed, spl);
		return pos[1] <= h;
	}
} // namespace Vox::Game::Generation::Perlins

#endif //__PERLIN_HPP__
