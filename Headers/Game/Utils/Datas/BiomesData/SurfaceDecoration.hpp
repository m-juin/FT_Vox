#ifndef __SURFACEDECORATION_HPP__
#define __SURFACEDECORATION_HPP__

#include "./DecorationRule.hpp"

#include <unordered_map>
#include <vector>

#include "../Biomes.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{
	struct SurfaceDecoration
	{
			const std::vector<SurfaceRule> rules;
	};

	static const std::unordered_map<const Biomes, const SurfaceDecoration> biomesSurfaces = {
		{Biomes::Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}}},
		{Biomes::Beach, {{{3, "Sand"}, {9999, "Stone"}}}},

		{Biomes::Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Cold_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Deep_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Warm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Frozen_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Lukewarm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Deep_Cold_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Deep_Frozen_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Deep_Lukewarm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
		{Biomes::Deep_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}}},
	};

	inline const std::string GetBlockType(Biomes biome, int depth)
	{
		if (biomesSurfaces.find(biome) == biomesSurfaces.end())
		{
            // if (depth <= 0)
                // return "Grass";
            // else if (depth <= 3)
                // return "Dirt";
            // else 
            return "Stone";
        }
		for (auto &r : biomesSurfaces.at(biome).rules)
		{
			if (depth <= r.surfaceDist)
				return r.blockType;
		}
		return "Stone";
	}
} // namespace Vox::Game::Generation::Datas::Biomes

#endif // __SURFACEDECORATION_HPP__