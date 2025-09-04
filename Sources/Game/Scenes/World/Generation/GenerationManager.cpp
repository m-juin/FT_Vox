#include "Game/Scenes/World/Generation/GenerationManager.hpp"

#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"

#include "Game/Scenes/World/WorldManager.hpp"
#include "Game/GameManager.hpp"
#include "Game/ThreadManager.hpp"

namespace Vox::Game::Generation
{
	GenerationManager::GenerationManager()
	{
		this->onUpdate.AddCallBack([this]() { this->UpdateGeneration(); });
	}

	void GenerationManager::RequestChuncksGeneration(World::Chuncks::VoxelChunck *ch)
	{
		if (std::find_if(
				this->_waitingChuncks.begin(), this->_waitingChuncks.end(), [ch](World::Chuncks::VoxelChunck *cluster)
				{ return (cluster->GetChunckPosition() == ch->GetChunckPosition()); }) != this->_waitingChuncks.end())
			return;
		this->_waitingChuncks.push_back(ch);
		Game::GameManager::GetInstance().GetThreadManager().EnQueue([ch](){
			ch->BuildVoxelObject();
		});
	}

	void GenerationManager::CancelChuncksGeneration(World::Chuncks::VoxelChunck *ch)
	{
		auto it = std::find(this->_waitingChuncks.begin(), this->_waitingChuncks.end(), ch);
		if (it != this->_waitingChuncks.end())
		{
			if (ch->GetGenerationState() == E_GenerationState::WaitingThread)
				this->_waitingChuncks.erase(it);
		}
	}

	void GenerationManager::UpdateGeneration()
	{
		std::list<World::Chuncks::VoxelChunck *> endedCluster;
		for (auto ch : this->_waitingChuncks)
		{
			if (ch->GetGenerationState() == Generation::E_GenerationState::WaitingBuffer)
				endedCluster.push_back(ch);
		}
		for (auto ch : endedCluster)
		{
			ch->BuildBufferObject();
			this->_waitingChuncks.erase(std::find(this->_waitingChuncks.begin(), this->_waitingChuncks.end(), ch));
		}
		World::WorldManager::GetInstance().AddEndedChunck(endedCluster);
	}
} // namespace Vox::Game::Generation