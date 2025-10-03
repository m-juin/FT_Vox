#ifndef __PERLINUTILS_HPP__
#define __PERLINUTILS_HPP__

#include <cstdint>

#include "MathGraphicalLib/Vectors/Vector2.hpp"

namespace Vox::Game::Generation::Perlins::Utils
{
	struct PerlinData
	{
			float amp;
			float freq;
			uint8_t octaves;
			MGL::Vectors::Vector2<float> offset;
			uint8_t splitted;

			constexpr PerlinData(float amp_, float freq_, uint8_t octaves_, MGL::Vectors::Vector2<float> offset_,
								 uint8_t splitted_)
				: amp(amp_), freq(freq_), octaves(octaves_), offset(offset_), splitted(splitted_)
			{
			}
	};
	constexpr PerlinData ContinentalnessData(1.3f, 0.0005f, 8, MGL::Vectors::Vector2<float>(0.0f, 0.0f), 6);
	constexpr PerlinData ErosionData(1.0f, 0.0002f, 8, MGL::Vectors::Vector2<float>(0.0f, 0.0f), 6);
	constexpr PerlinData PeaksAndValleyData(1.5f, 0.0006f, 8, MGL::Vectors::Vector2<float>(0.0f, 0.0f), 4);
	constexpr PerlinData HumidityData(1.0f, 0.0002f, 4, MGL::Vectors::Vector2<float>(1250.0f, -1250.0f), 4);
	constexpr PerlinData TemperatureData(1.3f, 0.00008f, 8, MGL::Vectors::Vector2<float>(-1250.0f, 1250.0f), 4);
	constexpr PerlinData WeirdnessData(1.5f, 0.0006f, 8, MGL::Vectors::Vector2<float>(0.0f, 0.0f), 4);

} // namespace Vox::Game::Generation::Perlins::Utils

#endif // __PERLINUTILS_HPP__