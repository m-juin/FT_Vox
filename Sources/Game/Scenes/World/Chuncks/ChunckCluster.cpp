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
			}
		}
		return cacheSt;
	}

	void ChunckCluster::Render(uint8_t toRender)
	{
		if (this->GetGenerationState() != Generation::E_GenerationState::End)
			return;
		for (auto ch : this->_clusterContent)
			if (ch)
				ch->Render(toRender);
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
		const std::vector<Game::Datas::Textures::TextureInfo> &textInfo,
		const std::vector<Game::Datas::Textures::TextureInfo> &transparenttextInfo, const uint32_t seed)
	{
		this->ChangeGenerationState(Generation::E_GenerationState::Mesh);

		auto st = GenerateCache(seed, spl);

		int chunksPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;
		for (int y = chunksPerCluster - 1; y >= 0; y--)
		{
			if (this->IsGenerationCancelled())
				return;
			this->_clusterContent[y] = new VoxelChunck(Vector3Int(this->_clusterPos[0], y, this->_clusterPos[1]));

			this->_clusterContent[y]->BuildVoxelObject(spl, textInfo, transparenttextInfo, st, seed);
		}
		this->GenerateClusterDecoration(spl, textInfo, transparenttextInfo, seed);
		this->ChangeGenerationState(Generation::E_GenerationState::WaitingBuffer);
	}

	void ChunckCluster::GenerateClusterDecoration(
		const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl,
		const std::vector<Game::Datas::Textures::TextureInfo> &textInfo,
		const std::vector<Game::Datas::Textures::TextureInfo> &transparenttextInfo, const uint32_t seed)
	{
		(void)spl;
		(void)textInfo;
		(void)transparenttextInfo;
		(void)seed;
	}

	ChunckCoord ChunckCluster::GetPosition()
	{
		return this->_clusterPos;
	}
	
	void ChunckCluster::SetBlock(MGL::Vectors::Vector3<uint8_t> localPos)
	{
		(void)localPos;
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
