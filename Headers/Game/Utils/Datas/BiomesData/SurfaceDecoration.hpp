#ifndef __SURFACEDECORATION_HPP__
#define __SURFACEDECORATION_HPP__

#include "./GenerationRules.hpp"

#include <unordered_map>
#include <vector>

#include "../Biomes.hpp"

#include "MathGraphicalLib/Vectors/Vector3.hpp"

// #include "Game/Scenes/World/Generation/RulesManager.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{
	struct SurfaceDecoration
	{
			const std::vector<SurfaceRule> surfaceRules;
			const HeightRule heightRules;
			// const MGL::Vectors::Vector3<float> grassColor;
			const bool haveWater;
			// const MGL::Vectors::Vector3<float> waterColor;
	};

	// static const std::unordered_map<const Biomes, const SurfaceDecoration> biomesSurfaces = {
		// {Biomes::Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Snowy_Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Sunflower_Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Plains, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Savanna, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Windswept_Savanna, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Desert, {{{9999, "Sand"}}, {0.55, 50}, true}},
		// {Biomes::Meadow, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Grove, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Cherry_Grove, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Snowy_Slopes, {{{9999, "Stone"}}, {0.6, 80}, true}},
		// {Biomes::Beach, {{{3, "Sand"}, {9999, "Stone"}}, {0.5, 57}, true}},
		// {Biomes::Stony_Shore, {{{9999, "Stone"}}, {0.5, 57}, true}},
		// {Biomes::Snowy_Beach, {{{9999, "Stone"}}, {0.5, 57}, true}},
		// {Biomes::Swamp, {{{9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Mangrove_Swamp, {{{9999, "Stone"}}, {0.55, 50}, true}},
// 
		// {Biomes::Savanna_Plateau, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.6, 60}, true}},
		// {Biomes::Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Birch_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Dark_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Flower_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Windswept_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Jungle, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Bamboo_Jungle, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Sparse_Jungle, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Taiga, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Snowy_Taiga, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
// 
		// {Biomes::Old_Growth_Birch_Forest, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Old_Growth_Pine_Taiga, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
		// {Biomes::Old_Growth_Spruce_Taiga, {{{0, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.55, 50}, true}},
// 
		// {Biomes::River, {{{9999, "Sand"}}, {0.5f, 40}, true}},
		// {Biomes::Frozen_River, {{{9999, "Gravel"}}, {0.5f, 40}, true}},
// 
		// {Biomes::Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 30}, true}},
		// {Biomes::Cold_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 30}, true}},
		// {Biomes::Deep_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 30}, true}},
		// {Biomes::Warm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 30}, true}},
		// {Biomes::Frozen_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 30}, true}},
		// {Biomes::Lukewarm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 30}, true}},
		// {Biomes::Deep_Cold_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 20}, true}},
		// {Biomes::Deep_Frozen_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 20}, true}},
		// {Biomes::Deep_Lukewarm_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 20}, true}},
		// {Biomes::Deep_Ocean, {{{5, "Gravel"}, {9999, "Stone"}}, {0.6f, 20}, true}},
// 
		// {Biomes::Mushroom_Fields, {{{1, "Grass"}, {3, "Dirt"}, {9999, "Stone"}}, {0.6f, 60}, true}},
// 
		// {Biomes::Jagged_Peaks, {{{9999, "Dirt"}}, {0.7f, 100}, true}},
		// {Biomes::Frozen_Peaks, {{{9999, "Stone"}}, {0.7f, 100}, true}},
		// {Biomes::Stony_Peaks, {{{9999, "Stone"}}, {0.7f, 100}, true}},
		// {Biomes::Windswept_Hills, {{{9999, "Stone"}}, {0.7f, 100}, true}},
		// {Biomes::Windswept_Gravelly_Hills, {{{9999, "Stone"}}, {0.7f, 100}, true}},
// 
	// };

	static const std::unordered_map<std::string, Biomes> biomesStringToEnum = {
		{"Plains", Biomes::Plains},
		{"Snowy_Plains", Biomes::Snowy_Plains},
		{"Sunflower_Plains", Biomes::Sunflower_Plains},
		{"Savanna", Biomes::Savanna},
		{"Windswept_Savanna", Biomes::Windswept_Savanna},
		{"Desert", Biomes::Desert},
		{"Meadow", Biomes::Meadow},
		{"Grove", Biomes::Grove},
		{"Cherry_Grove", Biomes::Cherry_Grove},
		{"Snowy_Slopes", Biomes::Snowy_Slopes},
		{"Beach", Biomes::Beach},
		{"Stony_Shore", Biomes::Stony_Shore},
		{"Snowy_Beach", Biomes::Snowy_Beach},
		{"Swamp", Biomes::Swamp},
		{"Mangrove_Swamp", Biomes::Mangrove_Swamp},
		{"Savanna_Plateau", Biomes::Savanna_Plateau},
		{"Forest", Biomes::Forest},
		{"Birch_Forest", Biomes::Birch_Forest},
		{"Dark_Forest", Biomes::Dark_Forest},
		{"Flower_Forest", Biomes::Flower_Forest},
		{"Windswept_Forest", Biomes::Windswept_Forest},
		{"Jungle", Biomes::Jungle},
		{"Bamboo_Jungle", Biomes::Bamboo_Jungle},
		{"Sparse_Jungle", Biomes::Sparse_Jungle},
		{"Taiga", Biomes::Taiga},
		{"Snowy_Taiga", Biomes::Snowy_Taiga},
		{"Old_Growth_Birch_Forest", Biomes::Old_Growth_Birch_Forest},
		{"Old_Growth_Pine_Taiga", Biomes::Old_Growth_Pine_Taiga},
		{"Old_Growth_Spruce_Taiga", Biomes::Old_Growth_Spruce_Taiga},
		{"River", Biomes::River},
		{"Frozen_River", Biomes::Frozen_River},
		{"Ocean", Biomes::Ocean},
		{"Cold_Ocean", Biomes::Cold_Ocean},
		{"Deep_Ocean", Biomes::Deep_Ocean},
		{"Warm_Ocean", Biomes::Warm_Ocean},
		{"Frozen_Ocean", Biomes::Frozen_Ocean},
		{"Lukewarm_Ocean", Biomes::Lukewarm_Ocean},
		{"Deep_Cold_Ocean", Biomes::Deep_Cold_Ocean},
		{"Deep_Frozen_Ocean", Biomes::Deep_Frozen_Ocean},
		{"Deep_Lukewarm_Ocean", Biomes::Deep_Lukewarm_Ocean},
		{"Mushroom_Fields", Biomes::Mushroom_Fields},
		{"Jagged_Peaks", Biomes::Jagged_Peaks},
		{"Frozen_Peaks", Biomes::Frozen_Peaks},
		{"Stony_Peaks", Biomes::Stony_Peaks},
		{"Windswept_Hills", Biomes::Windswept_Hills},
		{"Windswept_Gravelly_Hills", Biomes::Windswept_Gravelly_Hills}};

	// inline const std::string GetBlockType(Biomes biome, int depth)
	// {
		
	// 	if (biomesSurfaces.find(biome) == biomesSurfaces.end())
	// 	{
    //         // if (depth <= 0)
    //             // return "Grass";
    //         // else if (depth <= 3)
    //             // return "Dirt";
    //         // else 
    //         return "Stone";
    //     }
	// 	for (auto &r : biomesSurfaces.at(biome).surfaceRules)
	// 	{
	// 		if (depth <= r.surfaceDist)
	// 			return r.blockType;
	// 	}
	// 	return "Stone";
	// }

	// inline float GetHeight(Biomes biome, const float &val)
	// {
	// 	if (biomesSurfaces.find(biome) == biomesSurfaces.end())
	// 		return val;
	// 	return biomesSurfaces.at(biome).heightRules.Apply(val);
	// }
} // namespace Vox::Game::Generation::Datas::Biomes

#endif // __SURFACEDECORATION_HPP__