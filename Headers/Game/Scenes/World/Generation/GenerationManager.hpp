#ifndef __GENERATIONMANAGER_HPP__
#define __GENERATIONMANAGER_HPP__

#include "Utils/AUpdatable.hpp"
#include <memory>
#include <list>

#include "Game/Scenes/World/Utils/Defines.hpp"

namespace Vox::Game::World::Chuncks
{
	class ChunckCluster;
} // namespace Vox::Game::World::Chuncks

namespace Vox::Game::Generation
{
	class GenerationManager : public virtual Vox::Utils::AUpdatable
	{
		public:
			GenerationManager(const uint32_t seed);
			~GenerationManager() {};
			bool IsChunckPresent(const Vox::Game::Utils::Defines::ChunckCoord &coord);

            void RequestChuncksGeneration(Game::Utils::Defines::ChunckCoord coord);
			void SetPlayerPos(Vox::Game::Utils::Defines::ChunckCoord coord) {this->_playerPos = coord;};

			size_t GetWaitingData() const;

			uint32_t GetSeed() const {return this->_seed;} ;

		private:
			uint32_t _seed; 
			Vox::Game::Utils::Defines::ChunckCoord _playerPos;
            void UpdateGeneration();
			std::list<std::shared_ptr<World::Chuncks::ChunckCluster>> _waitingChuncks;
			/* private */
	};
} // namespace Vox::Game::Generation

#endif // __GENERATIONMANAGER_HPP__