#ifndef __BIOMES_HPP__
#define __BIOMES_HPP__

#include <unordered_map>
#include <fstream>

#include "MathGraphicalLib/Vectors/Vector3.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{

	enum class Biomes
	{
		Frozen_Ocean = 0,
		Deep_Frozen_Ocean = 1,
		Cold_Ocean = 2,
		Deep_Cold_Ocean = 3,
		Ocean = 4,
		Deep_Ocean = 5,
		Lukewarm_Ocean = 6,
		Deep_Lukewarm_Ocean = 7,
		Warm_Ocean = 8,
		Mushroom_Fields = 9,

		Jagged_Peaks = 20,
		Frozen_Peaks = 21,
		Stony_Peaks = 22,
		Meadow = 23,
		Cherry_Grove = 24,
		Grove = 25,
		Snowy_Slopes = 26,
		Windswept_Hills = 27,
		Windswept_Gravelly_Hills = 28,
		Windswept_Forest = 29,

		Forest = 40,
		Flower_Forest = 41,
		Taiga = 42,
		Old_Growth_Pine_Taiga = 43,
		Old_Growth_Spruce_Taiga = 44,
		Snowy_Taiga = 45,
		Birch_Forest = 46,
		Old_Growth_Birch_Forest = 47,
		Dark_Forest = 48,
		Jungle = 49,
		Sparse_Jungle = 50,
		Bamboo_Jungle = 51,
		Pale_Garden = 52,

		River = 60,
		Frozen_River = 61,
		Swamp = 62,
		Mangrove_Swamp = 63,
		Beach = 64,
		Snowy_Beach = 65,
		Stony_Shore = 66,

		Plains = 80,
		Sunflower_Plains = 81,
		Snowy_Plains = 82,
		Ice_Spikes = 83,

		Desert = 100,
		Savanna = 101,
		Savanna_Plateau = 102,
		Windswept_Savanna = 103,
		Badlands = 104,
		Wooded_Badlands = 105,
		Eroded_Badlands = 106,

		// < 20 = Offland
		// < 40 = Highland
		// < 60 Woodland
		// < 80 Wetland
		// < 100 Flat
		// < 120 Arid
	};

	inline std::ostream &operator<<(std::ostream &out, const Biomes value)
	{
		switch (value)
		{
#define PROCESS_VAL(p)                                                                                                 \
	case Biomes::p:                                                                                                    \
		return out << #p;

			PROCESS_VAL(Frozen_Ocean);
			PROCESS_VAL(Deep_Frozen_Ocean);
			PROCESS_VAL(Cold_Ocean);
			PROCESS_VAL(Deep_Cold_Ocean);
			PROCESS_VAL(Ocean);
			PROCESS_VAL(Deep_Ocean);
			PROCESS_VAL(Lukewarm_Ocean);
			PROCESS_VAL(Deep_Lukewarm_Ocean);
			PROCESS_VAL(Warm_Ocean);
			PROCESS_VAL(Mushroom_Fields);

			PROCESS_VAL(Jagged_Peaks);
			PROCESS_VAL(Frozen_Peaks);
			PROCESS_VAL(Stony_Peaks);
			PROCESS_VAL(Meadow);
			PROCESS_VAL(Cherry_Grove);
			PROCESS_VAL(Grove);
			PROCESS_VAL(Snowy_Slopes);
			PROCESS_VAL(Windswept_Hills);
			PROCESS_VAL(Windswept_Gravelly_Hills);
			PROCESS_VAL(Windswept_Forest);

			PROCESS_VAL(Forest);
			PROCESS_VAL(Flower_Forest);
			PROCESS_VAL(Taiga);
			PROCESS_VAL(Old_Growth_Pine_Taiga);
			PROCESS_VAL(Old_Growth_Spruce_Taiga);
			PROCESS_VAL(Snowy_Taiga);
			PROCESS_VAL(Birch_Forest);
			PROCESS_VAL(Old_Growth_Birch_Forest);
			PROCESS_VAL(Dark_Forest);
			PROCESS_VAL(Jungle);
			PROCESS_VAL(Sparse_Jungle);
			PROCESS_VAL(Bamboo_Jungle);
			PROCESS_VAL(Pale_Garden);

			PROCESS_VAL(River);
			PROCESS_VAL(Frozen_River);
			PROCESS_VAL(Swamp);
			PROCESS_VAL(Mangrove_Swamp);
			PROCESS_VAL(Beach);
			PROCESS_VAL(Snowy_Beach);
			PROCESS_VAL(Stony_Shore);

			PROCESS_VAL(Plains);
			PROCESS_VAL(Sunflower_Plains);
			PROCESS_VAL(Snowy_Plains);
			PROCESS_VAL(Ice_Spikes);

			PROCESS_VAL(Desert);
			PROCESS_VAL(Savanna);
			PROCESS_VAL(Savanna_Plateau);
			PROCESS_VAL(Windswept_Savanna);
			PROCESS_VAL(Badlands);
			PROCESS_VAL(Wooded_Badlands);
			PROCESS_VAL(Eroded_Badlands);

#undef PROCESS_VAL

		default:
			return out << "UNKNOWN";
		}
	}

	inline std::unordered_map<Biomes, MGL::Vectors::Vector3<int>> biomesColors = {
		{Biomes::Ocean, {0, 77, 153}},
		{Biomes::Deep_Ocean, {0, 51, 102}},
		{Biomes::Warm_Ocean, {0, 128, 153}},
		{Biomes::Lukewarm_Ocean, {0, 102, 128}},
		{Biomes::Deep_Lukewarm_Ocean, {0, 77, 102}},
		{Biomes::Cold_Ocean, {0, 77, 128}},
		{Biomes::Deep_Cold_Ocean, {0, 51, 77}},
		{Biomes::Frozen_Ocean, {153, 204, 255}},
		{Biomes::Deep_Frozen_Ocean, {102, 153, 230}},
		{Biomes::Mushroom_Fields, {153, 0, 153}},

		{Biomes::Jagged_Peaks, {204, 204, 230}},
		{Biomes::Frozen_Peaks, {230, 230, 255}},
		{Biomes::Stony_Peaks, {153, 153, 153}},
		{Biomes::Meadow, {102, 204, 102}},
		{Biomes::Cherry_Grove, {255, 179, 204}},
		{Biomes::Grove, {128, 179, 128}},
		{Biomes::Snowy_Slopes, {230, 230, 242}},
		{Biomes::Windswept_Hills, {128, 153, 102}},
		{Biomes::Windswept_Gravelly_Hills, {128, 128, 128}},
		{Biomes::Windswept_Forest, {102, 153, 102}},

		{Biomes::Forest, {26, 153, 26}},
		{Biomes::Flower_Forest, {204, 230, 153}},
		{Biomes::Taiga, {51, 128, 51}},
		{Biomes::Old_Growth_Pine_Taiga, {38, 102, 51}},
		{Biomes::Old_Growth_Spruce_Taiga, {31, 89, 51}},
		{Biomes::Snowy_Taiga, {204, 204, 230}},
		{Biomes::Birch_Forest, {153, 204, 102}},
		{Biomes::Old_Growth_Birch_Forest, {128, 179, 77}},
		{Biomes::Dark_Forest, {26, 77, 26}},
		{Biomes::Jungle, {0, 128, 0}},
		{Biomes::Sparse_Jungle, {77, 153, 77}},
		{Biomes::Bamboo_Jungle, {102, 179, 51}},
		{Biomes::Pale_Garden, {230, 242, 204}},

		{Biomes::River, {51, 102, 230}},
		{Biomes::Frozen_River, {179, 217, 255}},
		{Biomes::Swamp, {51, 77, 26}},
		{Biomes::Mangrove_Swamp, {26, 102, 51}},
		{Biomes::Beach, {230, 217, 153}},
		{Biomes::Snowy_Beach, {242, 242, 242}},
		{Biomes::Stony_Shore, {128, 128, 128}},

		{Biomes::Plains, {128, 204, 51}},
		{Biomes::Sunflower_Plains, {230, 230, 102}},
		{Biomes::Snowy_Plains, {242, 242, 255}},
		{Biomes::Ice_Spikes, {204, 230, 255}},

		{Biomes::Desert, {237, 217, 140}},
		{Biomes::Savanna, {204, 179, 51}},
		{Biomes::Savanna_Plateau, {179, 153, 38}},
		{Biomes::Windswept_Savanna, {191, 153, 51}},
		{Biomes::Badlands, {204, 77, 26}},
		{Biomes::Wooded_Badlands, {179, 64, 26}},
		{Biomes::Eroded_Badlands, {230, 102, 51}},
	};

} // namespace Vox::Game::Generation::Datas::Biomes

#endif // __BIOMES_HPP__