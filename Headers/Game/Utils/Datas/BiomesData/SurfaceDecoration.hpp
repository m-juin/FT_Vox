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
		{Biomes::Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Snowy_Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Sunflower_Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Savanna, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Windswept_Savanna, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Desert, {{{9999, "Sand"}}, {0.5, 60}}},
		{Biomes::Meadow, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.6, 60}}},
		{Biomes::Grove, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Cherry_Grove, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Snowy_Slopes, {{{9999, "Stone"}}, {0.5, 80}}},
		{Biomes::Beach, {{{3, "Sand"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Stony_Shore, {{{9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Snowy_Beach, {{{9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Swamp, {{{9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Mangrove_Swamp, {{{9999, "Stone"}}, {0.5, 60}}},

		{Biomes::Savanna_Plateau, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 70}}},
		{Biomes::Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Birch_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Dark_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Flower_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Windswept_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Jungle, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Bamboo_Jungle, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Sparse_Jungle, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Taiga, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},
		{Biomes::Snowy_Taiga, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.5, 60}}},

		{Biomes::Old_Growth_Birch_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.6, 60}}},
		{Biomes::Old_Growth_Pine_Taiga, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.6, 60}}},
		{Biomes::Old_Growth_Spruce_Taiga, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.6, 60}}},

		{Biomes::River, {{{9999, "Sand"}}, {0.5f, 50}}},
		{Biomes::Frozen_River, {{{9999, "Gravel"}}, {0.5f, 50}}},

		{Biomes::Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 30}}},
		{Biomes::Cold_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 30}}},
		{Biomes::Deep_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 30}}},
		{Biomes::Warm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 30}}},
		{Biomes::Frozen_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 30}}},
		{Biomes::Lukewarm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 30}}},
		{Biomes::Deep_Cold_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 20}}},
		{Biomes::Deep_Frozen_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 20}}},
		{Biomes::Deep_Lukewarm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 20}}},
		{Biomes::Deep_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.5f, 20}}},

		{Biomes::Mushroom_Fields, {{{1, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.6f, 60}}},

		{Biomes::Jagged_Peaks, {{{9999, "Dirt"}}, {0.7f, 100}}},
		{Biomes::Frozen_Peaks, {{{9999, "Stone"}}, {0.7f, 100}}},
		{Biomes::Stony_Peaks, {{{9999, "Stone"}}, {0.7f, 100}}},
		{Biomes::Windswept_Hills, {{{9999, "Stone"}}, {0.7f, 100}}},
		{Biomes::Windswept_Gravelly_Hills, {{{9999, "Stone"}}, {0.7f, 100}}},



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