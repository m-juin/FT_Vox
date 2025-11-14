#ifndef __CHUNK_OPERATIONS_HPP__
#define __CHUNK_OPERATIONS_HPP__

#include "MathGraphicalLib/Vectors/Vector3.hpp"

#include "Game/Scenes/World/Utils/Defines.hpp"

namespace Vox::Game::Chuncks::Operations
{
	using WorldCoord = MGL::Vectors::Vector3<int>;

	using ChunkCoord = MGL::Vectors::Vector3<uint8_t>;

	inline ChunkCoord WorldToChunk(WorldCoord coord)
	{
		ChunkCoord ret;
		for (int i = 0; i < 3; i++)
			ret[i] = coord[i] % static_cast<int>(Game::Utils::Defines::CHUNCK_SIZE);
		return ret;
	}
	inline WorldCoord ChunkToWorld(ChunkCoord coord, WorldCoord chunckPos)
	{
		WorldCoord ret;
		for (int i = 0; i < 3; i++)
            ret[i] = chunckPos[i] * Game::Utils::Defines::CHUNCK_SIZE + coord[i];
		return ret;
	}
} // namespace Vox::Game::Chuncks::Operations

#endif // __CHUNK_OPERATIONS_HPP__