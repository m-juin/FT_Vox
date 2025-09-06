#ifndef __CHUNCKCLUSTER_HPP__
#define __CHUNCKCLUSTER_HPP__

#include "../Utils/Defines.hpp"
#include "Game/Scenes/World/Generation/ThreadObject.hpp"

#include <array>

#include "VoxelChunck.hpp"

namespace Vox::Game::World::Chuncks
{
	class ChunckCluster : public Generation::Threads::ThreadObject, public Vox::Utils::AUpdatable
	{
		private:
			std::array<VoxelChunck *, WORLD_HEIGHT / CHUNCK_SIZE> _clusterContent;

			ChunckCoord _clusterPos;

		public:
			uint16_t _bufferIndex;
			uint16_t GetBuffer() const;
			void Render();
			void BuildClusterContent(const std::unordered_map<std::string, const Spline::Spline> &spl);
			ChunckCoord GetPosition();

			void BuildBuffers(const uint16_t &buffer);
			const uint8_t *GenerateHeightMap(const std::unordered_map<std::string, const Spline::Spline> &spl);

			ChunckCluster(const ChunckCoord &coord);
			~ChunckCluster();
	};
}
#endif //__CHUNCKCLUSTER_HPP__
