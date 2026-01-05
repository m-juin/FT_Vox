#ifndef __BLOCKSACCESSUTILITY_HPP__
#define __BLOCKSACCESSUTILITY_HPP__

#include "MathGraphicalLib/Vectors/Defines.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>

#include "./ChunckCluster.hpp"
#include "Game/Datas/Blocks/BlocksType.hpp"

namespace Vox::World::BlocksAccessUtility
{
	using namespace MGL::Vectors::Types;
	using blockType = Vox::Game::Datas::Blocks::BlockType;

	inline void SetBlock(Vector3Int wp,
						 std::unordered_map<const Vector2Int, std::shared_ptr<Game::World::Chuncks::ChunckCluster>,
											MGL::Vectors::Vector2Hash<int>> &&wd,
						 blockType type)
	{
		Vector2Int cp(std::floor(static_cast<float>(wp[0]) / static_cast<float>(Game::Utils::Defines::CHUNCK_SIZE)),
					  std::floor(static_cast<float>(wp[2]) / static_cast<float>(Game::Utils::Defines::CHUNCK_SIZE)));
		if (auto it = wd.find(cp); it != wd.end())
		{
			// it->second->GetChunk();
		}
	}
	// BlockData &GetBlock(Vector3Int wp);

} // namespace Vox::World::BlocksAccessUtility

#endif // __BLOCKSACCESSUTILITY_HPP__