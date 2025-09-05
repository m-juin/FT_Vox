#ifndef __GENERATIONMANAGER_HPP__
#define __GENERATIONMANAGER_HPP__

#include "Utils/AUpdatable.hpp"
#include <memory>
#include <list>

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

            void RequestChuncksGeneration(World::Chuncks::ChunckCluster *);
            void CancelChuncksGeneration(World::Chuncks::ChunckCluster *);

			size_t GetWaitingData() const;

		private:
            void UpdateGeneration();
			std::list<World::Chuncks::ChunckCluster *> _waitingChuncks;
			/* private */
	};
} // namespace Vox::Game::Generation

#endif // __GENERATIONMANAGER_HPP__