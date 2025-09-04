#ifndef __GENERATIONMANAGER_HPP__
#define __GENERATIONMANAGER_HPP__

#include "Utils/AUpdatable.hpp"
#include <memory>
#include <list>

namespace Vox::Game::World::Chuncks
{
	class VoxelChunck;
} // namespace Vox::Game::World::Chuncks

namespace Vox::Game::Generation
{
	class GenerationManager : public virtual Vox::Utils::AUpdatable
	{
		public:
			GenerationManager();
			~GenerationManager() {};

            void RequestChuncksGeneration(World::Chuncks::VoxelChunck *);
            void CancelChuncksGeneration(World::Chuncks::VoxelChunck *);

		private:
            void UpdateGeneration();
			std::list<World::Chuncks::VoxelChunck *> _waitingChuncks;
			/* private */
	};
} // namespace Vox::Game::Generation

#endif // __GENERATIONMANAGER_HPP__