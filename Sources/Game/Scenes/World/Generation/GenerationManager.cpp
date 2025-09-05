#include "Game/Scenes/World/Generation/GenerationManager.hpp"

#include "Game/Scenes/World/Chuncks/ChunckCluster.hpp"

#include "Game/Scenes/World/WorldManager.hpp"
#include "Game/Scenes/World/Generation/SplinesManager.hpp"
#include "Game/GameManager.hpp"
#include "Game/ThreadManager.hpp"

namespace Vox::Game::Generation
{
	GenerationManager::GenerationManager()
	{
		this->onUpdate.AddCallBack([this]() { this->UpdateGeneration(); });
	}

	void GenerationManager::RequestChuncksGeneration(World::Chuncks::ChunckCluster *ch)
	{
		if (std::find_if(
				this->_waitingChuncks.begin(), this->_waitingChuncks.end(), [ch](World::Chuncks::ChunckCluster *cluster)
				{ return (cluster->GetPosition() == ch->GetPosition()); }) != this->_waitingChuncks.end())
			return;
		this->_waitingChuncks.push_back(ch);
		Game::GameManager::GetInstance().GetThreadManager().EnQueue([ch](std::unordered_map<std::string, const Spline::Spline> spl){
			ch->BuildClusterContent(spl);
		});
	}

	void GenerationManager::CancelChuncksGeneration(World::Chuncks::ChunckCluster *ch)
	{
		auto it = std::find(this->_waitingChuncks.begin(), this->_waitingChuncks.end(), ch);
		if (it != this->_waitingChuncks.end())
		{
			if (ch->GetGenerationState() == E_GenerationState::WaitingThread)
				this->_waitingChuncks.erase(it);
		}
	}
	
	size_t GenerationManager::GetWaitingData() const
	{
		return this->_waitingChuncks.size();
	}

	void GenerationManager::UpdateGeneration()
	{
		std::list<World::Chuncks::ChunckCluster *> endedCluster;
		for (auto ch : this->_waitingChuncks)
		{
			if (ch->GetGenerationState() == Generation::E_GenerationState::WaitingBuffer)
				endedCluster.push_back(ch);
		}
		for (auto ch : endedCluster)
		{
			ch->BuildBuffers();
			this->_waitingChuncks.erase(std::find(this->_waitingChuncks.begin(), this->_waitingChuncks.end(), ch));
		}
		World::WorldManager::GetInstance().AddEndedChunck(endedCluster);
	}
} // namespace Vox::Game::Generation