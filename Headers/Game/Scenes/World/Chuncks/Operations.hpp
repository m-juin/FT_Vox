#ifndef __CHUNK_OPERATIONS_HPP__
#define __CHUNK_OPERATIONS_HPP__

#include "MathGraphicalLib/Vectors/Defines.hpp"


#include "Game/Scenes/World/Utils/Defines.hpp"

namespace Vox::Game::Chuncks::Operations
{
	using namespace MGL::Vectors::Types;

	inline Vector3Uint8 WorldToChunk(Vector3Int coord)
	{
		Vector3Uint8 ret;
		for (int i = 0; i < 3; i++)
			ret[i] = (coord[i] + static_cast<int>(Game::Utils::Defines::CHUNCK_SIZE)) % static_cast<int>(Game::Utils::Defines::CHUNCK_SIZE);
		return ret;
	}

	inline Vector3Int ChunkToWorld(Vector3Uint8 coord, Vector3Int chunckPos)
	{
		Vector3Int ret;
		for (int i = 0; i < 3; i++)
            ret[i] = chunckPos[i] * Game::Utils::Defines::CHUNCK_SIZE + coord[i];
		return ret;
	}

	inline Vector2Int WorldToCluster(Vector3Int blockPos)
	{
		Vector2Int ret(std::floor(static_cast<float>(blockPos[0]) / static_cast<float>(Game::Utils::Defines::CHUNCK_SIZE)), std::floor(static_cast<float>(blockPos[2]) / static_cast<float>(Game::Utils::Defines::CHUNCK_SIZE)));
		return ret;
	}

	inline Vector3Uint8 ClusterToChunk(Vector3Uint8 clPos)
	{
		return {clPos[0], std::floor(static_cast<float>(clPos[1]) / static_cast<float>(Game::Utils::Defines::CHUNCK_SIZE)), clPos[2]};
	}

} // namespace Vox::Game::Chuncks::Operations

#endif // __CHUNK_OPERATIONS_HPP__