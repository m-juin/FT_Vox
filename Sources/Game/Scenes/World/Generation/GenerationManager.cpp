#include "Game/Scenes/World/Generation/GenerationManager.hpp"

#include "Game/Scenes/World/Chuncks/ChunckCluster.hpp"

#include "Game/GameManager.hpp"
#include "Game/Scenes/World/Generation/SplinesManager.hpp"
#include "Game/Scenes/World/WorldManager.hpp"
#include "Game/ThreadManager.hpp"

#include "MathGraphicalLib/Vectors/Operations.hpp"

namespace Vox::Game::Generation
{
	GenerationManager::GenerationManager(const uint64_t seed) : _seed(seed)
	{
		this->onUpdate.AddCallBack([this]() { this->UpdateGeneration(); });
		this->onUpdate.AddCallBack([this]() { this->CheckForPoolRebuild();});
	}

	bool GenerationManager::IsChunckPresent(const Vox::Game::Utils::Defines::ChunckCoord &coord)
	{
		return std::find_if(this->_waitingChuncks.begin(), this->_waitingChuncks.end(),
							[coord](std::shared_ptr<World::Chuncks::ChunckCluster> cluster)
							{ return (cluster->GetPosition() == coord); }) != this->_waitingChuncks.end();
	}

	void GenerationManager::RequestChuncksGeneration(Game::Utils::Defines::ChunckCoord coord)
	{
		if (_needThreadRefresh == true)
			return ;
		std::shared_ptr<World::Chuncks::ChunckCluster> ch = std::make_shared<World::Chuncks::ChunckCluster>(coord);
		this->_waitingChuncks.push_back(ch);

		auto seed = this->_seed;
		Game::GameManager::GetInstance().GetThreadManager().EnQueue(
			[ch, seed](std::unordered_map<std::string, std::pair<const Spline::Spline, float>> spl, std::vector<Game::Utils::Textures::TextureInfo> textInfo) { ch->BuildClusterContent(spl, textInfo, seed);});
	}

	size_t GenerationManager::GetWaitingData() const
	{
		return this->_waitingChuncks.size();
	}
	
	bool GenerationManager::UpdateSeed(uint64_t newSeed)
	{
		if (newSeed == this->_seed)
			return false;
		_seed = newSeed;
		this->_waitingChuncks.clear();
		return true;
	}
	
	void GenerationManager::FlagPool()
	{
		this->_needThreadRefresh = true;
	}
	
	void GenerationManager::CheckForPoolRebuild()
	{
		if (this->_needThreadRefresh)
		{
			Game::GameManager::GetInstance().GetThreadManager().RecreatePool();
			this->_waitingChuncks.clear();
			Game::World::WorldManager::GetInstance().RequestChunckRefresh();
			this->_needThreadRefresh = false;
		}
	}

	void GenerationManager::UpdateGeneration()
	{
		if (_needThreadRefresh)
			return ;
		auto &wm = World::WorldManager::GetInstance();
		auto &bm = wm.GetBufferManager();
		std::list<std::shared_ptr<World::Chuncks::ChunckCluster>> toDeleteCluster;
		std::list<std::shared_ptr<World::Chuncks::ChunckCluster>> endedCluster;
		for (auto ch : this->_waitingChuncks)
		{
			auto genState = ch->GetGenerationState();
			if (genState == Generation::E_GenerationState::WaitingThread ||
				genState == Generation::E_GenerationState::WaitingBuffer)
			{
				auto playerDist = MGL::Vectors::Dist(this->_playerPos, ch->GetPosition());
				if (playerDist >= Vox::Game::Utils::Defines::SQUARE_RENDER_DISTANCE)
				{
					ch->CancelGeneration();
					toDeleteCluster.push_back(ch);
					continue;
				}
			}
			if (genState == Generation::E_GenerationState::WaitingBuffer)
				endedCluster.push_back(ch);
		}
		for (auto ch : toDeleteCluster)
		{
			auto it = std::find(this->_waitingChuncks.begin(), this->_waitingChuncks.end(), ch);
			if (it != this->_waitingChuncks.end())
				this->_waitingChuncks.erase(it);
		}
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