#ifndef __CHUNCKCLUSTER_HPP__
#define __CHUNCKCLUSTER_HPP__

#include "../Utils/Defines.hpp"
#include "Game/Scenes/World/Generation/ThreadObject.hpp"

#include <array>

#include "VoxelChunck.hpp"
#include "Game/Utils/TexturesData.hpp"

#include "Game/Scenes/World/Generation/Utils.hpp"

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
			Game::Generation::Utils::ChunckCache GenerateCache(const uint32_t seed, const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl);
			void Render(uint8_t toRender);
			void BuildClusterContent(const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl, const std::vector<Game::Utils::Textures::TextureInfo> &textInfo, const std::vector<Game::Utils::Textures::TextureInfo> &transparenttextInfo, const uint32_t seed);
			ChunckCoord GetPosition();

			void BuildBuffers(const uint16_t &buffer);
			void GenerateHeightMap(const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl, const uint32_t seed, uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE]);

			ChunckCluster(const ChunckCoord &coord);
			~ChunckCluster();
	};
}
#endif //__CHUNCKCLUSTER_HPP__
