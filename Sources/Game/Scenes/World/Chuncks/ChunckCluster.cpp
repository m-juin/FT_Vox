#include "Game/Scenes/World/Chuncks/ChunckCluster.hpp"

#include "Game/Scenes/World/Generation/PerlinInterpretation.hpp"

#include "Game/Scenes/World/Generation/PerlinUtils.hpp"

namespace Vox::Game::World::Chuncks
{
	uint16_t ChunckCluster::GetBuffer() const
	{
		return _bufferIndex;
	}

	Game::Generation::Utils::ChunckCache ChunckCluster::GenerateCache(
		const uint32_t seed, const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl)
	{
		Game::Generation::Utils::ChunckCache cacheSt;

		int baseX = static_cast<int>(this->_clusterPos[0] * CHUNCK_SIZE);
		int baseZ = static_cast<int>(this->_clusterPos[1] * CHUNCK_SIZE);

		bool needPrint = this->_clusterPos[0] >= -45 && this->_clusterPos[0] <= -43 && this->_clusterPos[1] >= 19 && this->_clusterPos[1] <= 21;

		for (int x = -Game::Generation::Utils::GENERATION_BLEND_RADIUS;
			 x < static_cast<int>(CHUNCK_SIZE) + Game::Generation::Utils::GENERATION_BLEND_RADIUS; x++)
		{
			for (int z = -Game::Generation::Utils::GENERATION_BLEND_RADIUS;
				 z < static_cast<int>(CHUNCK_SIZE) + Game::Generation::Utils::GENERATION_BLEND_RADIUS; z++)
			{
				size_t localIndex =
					(x + Game::Generation::Utils::GENERATION_BLEND_RADIUS) * Game::Generation::Utils::CACHE_SIZE +
					(z + Game::Generation::Utils::GENERATION_BLEND_RADIUS);

				int worldX = baseX + x;
				int worldZ = baseZ + z;

				auto i = Generation::Perlins::GetBiomeInfoAtPoint(worldX, worldZ, seed);

				cacheSt.continental[localIndex] = i.Continental;
				cacheSt.erosion[localIndex] = i.Erosion;
				cacheSt.peaks[localIndex] = i.PV;
				cacheSt.biome[localIndex] = i.Biome;
			}
		}
		for (int x = -Game::Generation::Utils::GENERATION_BLEND_RADIUS;
			 x < static_cast<int>(CHUNCK_SIZE) + Game::Generation::Utils::GENERATION_BLEND_RADIUS; x++)
		{
			for (int z = -Game::Generation::Utils::GENERATION_BLEND_RADIUS;
				 z < static_cast<int>(CHUNCK_SIZE) + Game::Generation::Utils::GENERATION_BLEND_RADIUS; z++)
			{
				size_t localIndex =
					(x + Game::Generation::Utils::GENERATION_BLEND_RADIUS) * Game::Generation::Utils::CACHE_SIZE +
					(z + Game::Generation::Utils::GENERATION_BLEND_RADIUS);
				cacheSt.heightMap[localIndex] = Generation::Perlins::GetBlendedCached(x, z, spl, cacheSt);
				if (needPrint)
					std::cout << (int)cacheSt.heightMap[localIndex] << " ";
			}
			if (needPrint)
					std::cout << "\n";
		}
		return cacheSt;
	}

	void ChunckCluster::Render()
	{
		if (this->GetGenerationState() != Generation::E_GenerationState::End)
			return;
		for (auto ch : this->_clusterContent)
			if (ch)
				ch->Render();
	}

	void ChunckCluster::GenerateHeightMap(
		const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl, const uint32_t seed,
		uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE])
	{
		int baseX = static_cast<int>(this->_clusterPos[0] * CHUNCK_SIZE);
		int baseZ = static_cast<int>(this->_clusterPos[1] * CHUNCK_SIZE);

		for (size_t x = 0; x < CHUNCK_SIZE; x++)
		{
			for (size_t z = 0; z < CHUNCK_SIZE; z++)
			{
				hMap[x * CHUNCK_SIZE + z] = Generation::Perlins::GetHeightAt(baseX + x, baseZ + z, seed, spl);
			}
		}
	}

	void ChunckCluster::BuildClusterContent(
		const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl,
		const std::vector<Game::Utils::Textures::TextureInfo> &textInfo, const uint32_t seed)
	{
		this->ChangeGenerationState(Generation::E_GenerationState::Mesh);

		// uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE];
		// GenerateHeightMap(spl, seed, hMap);
		auto st = GenerateCache(seed, spl);

		int chunksPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;
		for (int y = chunksPerCluster - 1; y >= 0; y--)
		{
			if (this->IsGenerationCancelled())
				return;
			this->_clusterContent[y] = new VoxelChunck(Vector3Int(this->_clusterPos[0], y, this->_clusterPos[1]));

			this->_clusterContent[y]->BuildVoxelObject(spl, textInfo, st, seed);
		}
		this->ChangeGenerationState(Generation::E_GenerationState::WaitingBuffer);
	}

	ChunckCoord ChunckCluster::GetPosition()
	{
		return this->_clusterPos;
	}

	void ChunckCluster::BuildBuffers(const uint16_t &buffer)
	{
		this->_bufferIndex = buffer;
		int chunksPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;
		for (int y = chunksPerCluster - 1; y >= 0; y--)
		{
			int localIndex = chunksPerCluster - 1 - y;
			int globalIndex = _bufferIndex * chunksPerCluster + localIndex;
			auto ch = this->_clusterContent[y];
			if (ch)
				ch->BuildBufferObject(globalIndex);
		}
		this->_currentState = Generation::E_GenerationState::End;
	}

	ChunckCluster::ChunckCluster(const ChunckCoord &coord) : _clusterContent{}
	{
		_clusterPos = coord;
		this->onUpdate.AddCallBack(
			[this]()
			{
				for (auto ch : this->_clusterContent)
					if (ch)
						ch->Update();
			});
		this->_currentState = Generation::E_GenerationState::WaitingThread;
	}

	ChunckCluster::~ChunckCluster()
	{
		for (auto ch : this->_clusterContent)
		{
			if (ch)
				delete ch;
		}
	}
} // namespace Vox::Game::World::Chuncks
