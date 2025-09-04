//
// Created by mjuin on 9/4/25.
//

#include "Game/Scenes/World/Chuncks/ChunckCluster.hpp"

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

	void ChunckCluster::BuildClusterContent(const std::unordered_map<std::string, const Spline::Spline> &spl)
	{
		this->ChangeGenerationState(Generation::E_GenerationState::Mesh);
		int chunksPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;

		for (int y = chunksPerCluster - 1; y >= 0; y--)
		{
			int localIndex = chunksPerCluster - 1 - y;
			int globalIndex = _bufferIndex * chunksPerCluster + localIndex;

			this->_clusterContent[y] =
				new VoxelChunck(globalIndex, Vector3Int(this->_clusterPos[0], y, this->_clusterPos[1]));

			this->_clusterContent[y]->BuildVoxelObject(spl);
		}
		this->ChangeGenerationState(Generation::E_GenerationState::WaitingBuffer);
	}

	ChunckCoord ChunckCluster::GetPosition()
	{
		return this->_clusterPos;
	}

	void ChunckCluster::BuildBuffers()
	{
		for (auto ch : this->_clusterContent)
		{
			if (ch)
				ch->BuildBufferObject();
		}
		this->_currentState = Generation::E_GenerationState::End;
	}

	ChunckCluster::ChunckCluster(const ChunckCoord &coord, uint16_t buffer)
	{
		_clusterPos = coord;
		_bufferIndex = buffer;
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
