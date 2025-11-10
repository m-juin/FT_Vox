#ifndef __GENERATIONMANAGER_HPP__
#define __GENERATIONMANAGER_HPP__

#include "Utils/AUpdatable.hpp"
#include <memory>
#include <list>

#include "Game/Scenes/World/Utils/Defines.hpp"
#include "./ChunkOverflowManager.hpp"
#include "Game/Datas/Structures/StructuresManager.hpp"

namespace Vox::Game::World::Chuncks
{
	class ChunckCluster;
} // namespace Vox::Game::World::Chuncks

namespace Vox::Game::Generation
{
	class GenerationManager : public virtual Vox::Utils::AUpdatable
	{
		public:
			GenerationManager(const uint64_t seed);
			~GenerationManager() {};
			bool IsChunckPresent(const Vox::Game::Utils::Defines::ChunckCoord &coord);

            void RequestChuncksGeneration(Game::Utils::Defines::ChunckCoord coord);
			void SetPlayerPos(Vox::Game::Utils::Defines::ChunckCoord coord) {this->_playerPos = coord;};

			size_t GetWaitingData() const;

			uint64_t GetSeed() const {return this->_seed;} ;
			const Game::Datas::Structures::StructuresManager *GetStructuresManager() {return this->_sManager.get();} ;
			bool UpdateSeed(uint64_t newSeed);
			bool GetThreadRefreshState() {return this->_needThreadRefresh;};

			void FlagPool();

		private:
			std::unique_ptr<Game::Generation::ChunkOverflowManager> _oManager;
			std::unique_ptr<Game::Datas::Structures::StructuresManager> _sManager;
			void CheckForPoolRebuild();
			bool _needThreadRefresh = false;
			uint64_t _seed; 
			Vox::Game::Utils::Defines::ChunckCoord _playerPos;
            void UpdateGeneration();
			std::list<std::shared_ptr<World::Chuncks::ChunckCluster>> _waitingChuncks;
			/* private */
	};
} // namespace Vox::Game::Generation

#endif // __GENERATIONMANAGER_HPP__