#ifndef __GENUTILS_HPP__
#define __GENUTILS_HPP__

#include <iostream>

#include <random>

#include "Game/Utils/Datas/Biomes.hpp"
#include "Game/Scenes/World/Utils/Defines.hpp"

namespace Vox::Game::Generation::Utils
{
	inline const std::string GenerateSeed()
	{
		return std::to_string(rand());
	}

	constexpr uint8_t GENERATION_BLEND_RADIUS = 4;
	constexpr size_t CACHE_SIZE = Game::Utils::Defines::CHUNCK_SIZE + 2 * GENERATION_BLEND_RADIUS;

	struct ChunckCache
	{
			float continental[CACHE_SIZE * CACHE_SIZE];
			float erosion[CACHE_SIZE * CACHE_SIZE];
			float peaks[CACHE_SIZE * CACHE_SIZE];
			uint8_t heightMap[CACHE_SIZE * CACHE_SIZE];
			Game::Generation::Datas::Biomes::Biomes biome[CACHE_SIZE * CACHE_SIZE];
	};

} // namespace Vox::Game::Generation::Utils

#endif // __GENUTILS_HPP__