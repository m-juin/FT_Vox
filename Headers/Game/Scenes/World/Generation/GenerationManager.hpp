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
			GenerationManager();
			~GenerationManager() {};
			bool IsChunckPresent(const Vox::Game::Utils::Defines::ChunckCoord &coord);

            void RequestChuncksGeneration(World::Chuncks::ChunckCluster *);
            void CancelChuncksGeneration(World::Chuncks::ChunckCluster *);
			void SetPlayerPos(Vox::Game::Utils::Defines::ChunckCoord coord) {this->_playerPos = coord;};

			size_t GetWaitingData() const;

		private:
			Vox::Game::Utils::Defines::ChunckCoord _playerPos;
            void UpdateGeneration();
			std::list<World::Chuncks::ChunckCluster *> _waitingChuncks;
			/* private */
	};
} // namespace Vox::Game::Generation

#endif // __GENERATIONMANAGER_HPP__