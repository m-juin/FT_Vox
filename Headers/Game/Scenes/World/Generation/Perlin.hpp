//
// Created by mjuin on 9/6/25.
//

#ifndef __PERLIN_HPP__
#define __PERLIN_HPP__
#include <cstdint>

#include "Spline/Spline.hpp"

namespace Vox::World::Generation::Perlin
{
	static GetPerlinValue(float x, float y, uint32_t seed, PerlinData data, std::pair<double, double> range)
	{
		float val = 0.0f;

		float amp = data.amp;
		float freq = data.freq;
		float maxAmp = 0.0f;

		for (int counter = 0; counter < data.octaves; counter++)
		{
			val += amp * (perlin((x + data.offset.x) * freq, (y + data.offset.y) * freq, seed));
			freq *= 2;
			maxAmp += amp;
			amp /= 2;
		}
		// val = val / maxAmp;
		// val = valueRange.x + (val - -1.0f) * (valueRange.y - valueRange.x) / (1.0f - -1.0f);
		// val = ((val -1.0f) / (1.0f - 1.0f)) * (valueRange.y - valueRange.x) * valueRange.x;
		return Spline::GetNormalizedRangedValue(val, {-1.0f, 1.0f}, range);
	}
}


#endif //__PERLIN_HPP__
