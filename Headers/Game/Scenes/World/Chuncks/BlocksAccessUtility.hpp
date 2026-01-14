#ifndef __BLOCKSACCESSUTILITY_HPP__
#define __BLOCKSACCESSUTILITY_HPP__

#include "MathGraphicalLib/Vectors/Defines.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>

#include "./ChunckCluster.hpp"
#include "Game/Datas/Blocks/BlocksType.hpp"

#include "./Operations.hpp"

namespace Vox::World::BlocksAccessUtility
{
	using namespace MGL::Vectors::Types;
	using blockType = Vox::Game::Datas::Blocks::BlockType;

	/// @brief Set block type from its chunk position.
	/// @param cp The block chunk position.
	/// @param cd The chunk datas.
	/// @param nType The block new type.
	inline void SetBlockType(Vector3Uint8 cp, Game::World::Chuncks::VoxelChunck &cd, blockType nType)
	{
		auto data = cd.GetBlockData(cp);
		if (data.type == nType)
			return;
		data.type = nType;
		// TODO Request Mesh Rebuild
	}

	/// @brief Set block type from its cluster position.
	/// @param cp The block cluster position.
	/// @param cd The cluster datas.
	/// @param nType The block new type.
	inline void SetBlockType(Vector3Uint8 cp, std::shared_ptr<Game::World::Chuncks::ChunckCluster> &cd, blockType nType)
	{
		uint8_t localHeight = std::floor(static_cast<float>(cp[1]) / Game::Utils::Defines::CHUNCK_SIZE);
		auto chunk = cd->GetChunkFromlocal(localHeight);
		if (cd == nullptr)
		{
			// TODO Log Error
			return;
		}
		SetBlockType(Vox::Game::Chuncks::Operations::ClusterToChunk(cp), *chunk, nType);
	}

	/// @brief Set block type from its world position.
	/// @param wp The block world position.
	/// @param wd The world datas.
	/// @param nType The block new type.
	inline void SetBlockType(Vector3Int wp,
							 std::unordered_map<const Vector2Int, std::shared_ptr<Game::World::Chuncks::ChunckCluster>,
												MGL::Vectors::Vector2Hash<int>> &&wd,
							 blockType nType)
	{
		Vector2Int clPos = Vox::Game::Chuncks::Operations::WorldToCluster(wp);

		if (auto it = wd.find(clPos); it != wd.end())
		{
			SetBlockType(Vector3Uint8(clPos[0], wp[1], clPos[2]), it->second, nType);
		}
	}

	/// @brief Get block type from its chunk position.
	/// @param cp The block chunk position.
	/// @param cd The chunk datas.
	/// @return The block type.
	inline blockType GetBlockType(Vector3Uint8 cp, Game::World::Chuncks::VoxelChunck &cd)
	{
		auto data = cd.GetBlockData(cp);
		return data.type;
	}

	/// @brief Get block type from its cluster position.
	/// @param cp The block cluster position.
	/// @param cd The cluster datas.
	/// @return The block type.
	inline blockType GetBlockType(Vector3Uint8 cp, std::shared_ptr<Game::World::Chuncks::ChunckCluster> &cd)
	{
		uint8_t localHeight = std::floor(static_cast<float>(cp[1]) / Game::Utils::Defines::CHUNCK_SIZE);
		auto chunk = cd->GetChunkFromlocal(localHeight);
		if (cd == nullptr)
		{
			// TODO Log Error
			return blockType::Air;
		}
		return GetBlockType(Vox::Game::Chuncks::Operations::ClusterToChunk(cp), *chunk);
	}

	/// @brief Get a block type from its world position.
	/// @param wp The block world position.
	/// @param wd The world datas.
	/// @return The block type.
	inline blockType GetBlockType(
		Vector3Int wp, std::unordered_map<const Vector2Int, std::shared_ptr<Game::World::Chuncks::ChunckCluster>,
										  MGL::Vectors::Vector2Hash<int>> &&wd)
	{
		Vector2Int clPos(std::floor(static_cast<float>(wp[0]) / static_cast<float>(Game::Utils::Defines::CHUNCK_SIZE)),
						 std::floor(static_cast<float>(wp[2]) / static_cast<float>(Game::Utils::Defines::CHUNCK_SIZE)));
		if (auto it = wd.find(clPos); it != wd.end())
		{
			return GetBlockType(Vector3Uint8(clPos[0], wp[1], clPos[2]), it->second);
		}
		else
			return blockType::Air;
	}

} // namespace Vox::World::BlocksAccessUtility

#endif // __BLOCKSACCESSUTILITY_HPP__