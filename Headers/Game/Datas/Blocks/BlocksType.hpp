#ifndef __BLOCKSTYPE_HPP__
#define __BLOCKSTYPE_HPP__

#include <map>
#include <string>

namespace Vox::Game::Datas::Blocks
{
	enum class BlockType
	{
		DEBUG = -5,

		Air = -1,
		Dirt = 0,
		Grass = 1,
		Stone = 2,
		Sand = 3,
		Gravel = 4,

		Oak_Log = 35,


		Water = 1001,

		Oak_Leaves = 1002,
	};

	static const std::map<BlockType, std::string_view> EnumToString = {
		{BlockType::Air, "Air"},	 {BlockType::Dirt, "Dirt"},	 {BlockType::Grass, "Grass"},
		{BlockType::Stone, "Stone"}, {BlockType::Sand, "Sand"},	 {BlockType::Gravel, "Gravel"},
		{BlockType::Water, "Water"}, {BlockType::Oak_Log, "Oak_Log"}, {BlockType::Oak_Leaves, "Oak_Leaves"}, {BlockType::DEBUG, "Debug"}};

	static const std::map<std::string_view, BlockType> StringToEnum = []()
	{
		std::map<std::string_view, BlockType> map;
		for (const auto &[key, value] : EnumToString)
		{
			map[value] = key;
		}
		return map;
	}();

} // namespace Vox::Game::Datas::Blocks

#endif // __BLOCKSTYPE_HPP__