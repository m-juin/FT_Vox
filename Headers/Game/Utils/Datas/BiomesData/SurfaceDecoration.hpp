#ifndef __SURFACEDECORATION_HPP__
#define __SURFACEDECORATION_HPP__

#include "./GenerationRules.hpp"

#include <unordered_map>
#include <vector>

#include "../Biomes.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{
	struct SurfaceDecoration
	{
			const std::vector<SurfaceRule> surfaceRules;
			const HeightRule heightRules;
	};

	static const std::unordered_map<const Biomes, const SurfaceDecoration> biomesSurfaces = {
		{Biomes::Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {1.0, 80}}},
		{Biomes::Beach, {{{3, "Sand"}, {9999, "Stone"}}, {1.0, 80}}},

		{Biomes::Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {1.0, 80}}},
		{Biomes::Birch_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {1.0, 80}}},
		{Biomes::Dark_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {1.0, 80}}},
		{Biomes::Flower_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {1.0, 80}}},
		{Biomes::Windswept_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {1.0, 80}}},
		{Biomes::Jungle, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {1.0, 80}}},

		{Biomes::River, {{{9999, "Sand"}}, {1.f, 75}}},
		{Biomes::Frozen_River, {{{9999, "Gravel"}}, {1.f, 75}}},

		{Biomes::Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Cold_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Deep_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Warm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Frozen_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Lukewarm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Deep_Cold_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Deep_Frozen_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Deep_Lukewarm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
		{Biomes::Deep_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {1.0f, 50}}},
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
		for (auto &r : biomesSurfaces.at(biome).surfaceRules)
		{
			if (depth <= r.surfaceDist)
				return r.blockType;
		}
		return "Stone";
	}

	inline float GetHeight(Biomes biome, const float &val)
	{
		if (biomesSurfaces.find(biome) == biomesSurfaces.end())
			return val;
		return biomesSurfaces.at(biome).heightRules.Apply(val);
	}
} // namespace Vox::Game::Generation::Datas::Biomes

#endif // __SURFACEDECORATION_HPP__