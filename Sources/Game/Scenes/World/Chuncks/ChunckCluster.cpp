#include "Game/Scenes/World/Chuncks/ChunckCluster.hpp"

#include "Game/Scenes/World/Generation/PerlinInterpretation.hpp"

#include "Game/Scenes/World/Generation/PerlinUtils.hpp"

namespace Vox::Game::World::Chuncks
{
	uint16_t ChunckCluster::GetBuffer() const
	{
		return _bufferIndex;
	}

	void ChunckCluster::Render()
	{
		if (this->GetGenerationState() != Generation::E_GenerationState::End)
			return;
		for (auto ch : this->_clusterContent)
			if (ch)
				ch->Render();
	}

	void ChunckCluster::GenerateHeightMap(const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl,
										  const uint32_t seed, uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE])
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

	void ChunckCluster::BuildClusterContent(const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl, const std::vector<Game::Utils::Textures::TextureInfo> &textInfo,
											const uint32_t seed)
	{
		this->ChangeGenerationState(Generation::E_GenerationState::Mesh);

		uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE];
		GenerateHeightMap(spl, seed, hMap);

		int chunksPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;
		for (int y = chunksPerCluster - 1; y >= 0; y--)
		{
			if (this->IsGenerationCancelled())
				return;
			this->_clusterContent[y] = new VoxelChunck(Vector3Int(this->_clusterPos[0], y, this->_clusterPos[1]));

			this->_clusterContent[y]->BuildVoxelObject(spl, textInfo, hMap, seed);
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
