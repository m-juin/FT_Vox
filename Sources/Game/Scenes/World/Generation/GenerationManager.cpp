#include "Game/Scenes/World/Generation/GenerationManager.hpp"

#include "Game/Scenes/World/Chuncks/ChunckCluster.hpp"

#include "Game/GameManager.hpp"
#include "Game/Scenes/World/Generation/SplinesManager.hpp"
#include "Game/Scenes/World/WorldManager.hpp"
#include "Game/ThreadManager.hpp"

#include "MathGraphicalLib/Vectors/Operations.hpp"

namespace Vox::Game::Generation
{
	GenerationManager::GenerationManager()
	{
		this->onUpdate.AddCallBack([this]() { this->UpdateGeneration(); });
	}

	bool GenerationManager::IsChunckPresent(const Vox::Game::Utils::Defines::ChunckCoord &coord)
	{
		return std::find_if(this->_waitingChuncks.begin(), this->_waitingChuncks.end(),
							[coord](World::Chuncks::ChunckCluster *cluster)
							{ return (cluster->GetPosition() == coord); }) != this->_waitingChuncks.end();
	}

	void GenerationManager::RequestChuncksGeneration(World::Chuncks::ChunckCluster *ch)
	{
		this->_waitingChuncks.push_back(ch);
		Game::GameManager::GetInstance().GetThreadManager().EnQueue(
			[ch](std::unordered_map<std::string, const Spline::Spline> spl) { ch->BuildClusterContent(spl); });
	}

	void GenerationManager::CancelChuncksGeneration(World::Chuncks::ChunckCluster *ch)
	{
		(void)ch;
		// auto it = std::find(this->_waitingChuncks.begin(), this->_waitingChuncks.end(), ch);
		// if (it != this->_waitingChuncks.end())
		// {
		// 	if (ch->GetGenerationState() == E_GenerationState::WaitingThread)
		// 		this->_waitingChuncks.erase(it);
		// }
	}

	size_t GenerationManager::GetWaitingData() const
	{
		return this->_waitingChuncks.size();
	}

	void GenerationManager::UpdateGeneration()
	{
		auto &wm = World::WorldManager::GetInstance();
		auto &bm = wm.GetBufferManager();
		std::list<World::Chuncks::ChunckCluster *> endedCluster;
		std::list<World::Chuncks::ChunckCluster *> toDeleteCluster;
		for (auto ch : this->_waitingChuncks)
		{
			auto genState = ch->GetGenerationState();
			if (genState == Generation::E_GenerationState::WaitingThread ||
				genState == Generation::E_GenerationState::WaitingBuffer)
			{
				auto playerDist = MGL::Vectors::Dist(this->_playerPos, ch->GetPosition());
				if (playerDist >= Vox::Game::Utils::Defines::HALF_RENDER_DISTANCE_SQUARE)
				{
					toDeleteCluster.push_back(ch);
					continue;
				}
			}
			if (genState == Generation::E_GenerationState::WaitingBuffer)
				endedCluster.push_back(ch);
		}
		// for (auto ch : toDeleteCluster)
		// {
			// auto it = std::find(this->_waitingChuncks.begin(), this->_waitingChuncks.end(), ch);
			// if (it != this->_waitingChuncks.end())
			// 	this->_waitingChuncks.erase(it);
			// delete ch;
		// }
		for (auto ch : endedCluster)
		{
			auto buffer = bm.ReserveBuffer();
			if (buffer < Vox::Game::Utils::Defines::CHUNCK_BUFFER_AMOUNT)
			{
				ch->BuildBuffers(buffer);
				this->_waitingChuncks.erase(std::find(this->_waitingChuncks.begin(), this->_waitingChuncks.end(), ch));
				wm.AddEndedChunck(ch);
			}
		}
	}
} // namespace Vox::Game::Generation