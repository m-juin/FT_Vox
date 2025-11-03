#ifndef __BLOCKSTYPE_HPP__
#define __BLOCKSTYPE_HPP__

#include <map>

namespace Vox::Game::Datas::Blocks
{
	enum class BlockType
	{
		Air = -1,
		Dirt = 0,
		Grass = 1,
		Stone = 2,
		Sand = 3,
		Gravel = 4,

		DEBUG= -5,

		Water = 1001,
	};

	static const std::map<BlockType, std::string_view> EnumToString = {
		{BlockType::Air, "Air"},
		{BlockType::Dirt, "Dirt"},
		{BlockType::Grass, "Grass"},
		{BlockType::Stone, "Stone"},
		{BlockType::Sand, "Sand"},
		{BlockType::Gravel, "Gravel"},
		{BlockType::Water, "Water"},
		{BlockType::DEBUG, "Debug"}
	};

} // namespace Vox::Game::Datas::Blocks

#endif // __BLOCKSTYPE_HPP__