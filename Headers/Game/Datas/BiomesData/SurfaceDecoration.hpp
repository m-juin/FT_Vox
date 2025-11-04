#ifndef __SURFACEDECORATION_HPP__
#define __SURFACEDECORATION_HPP__

#include "./GenerationRules.hpp"

#include <unordered_map>
#include <vector>
#include <algorithm>

#include "../Biomes.hpp"

#include "MathGraphicalLib/Vectors/Vector3.hpp"


namespace Vox::Game::Generation::Datas::Biomes
{

	struct SurfaceDecoration
	{
			const std::vector<SurfaceRule> surfaceRules;
			const HeightRule heightRules;
			const bool haveWater;
			const double treeChance;

			SurfaceDecoration(std::vector<SurfaceRule> surfaceRules_, HeightRule heightRules_, bool haveWater_,
							  double treeChance_)
				: surfaceRules(surfaceRules_), heightRules(heightRules_), haveWater(haveWater_),
				  treeChance(treeChance_)
			{
			}
	};

	using Biomes = Game::Datas::Biomes::Biomes;

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

} // namespace Vox::Game::Generation::Datas::Biomes

#endif // __SURFACEDECORATION_HPP__