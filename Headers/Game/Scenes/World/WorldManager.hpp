#ifndef __WORLDMANAGER_HPP__
#define __WORLDMANAGER_HPP__

#include "Game/Scenes/World/Utils/Defines.hpp"
#include "Game/Scenes/World/Utils/Vulkan.hpp"

#include <array>
#include <vulkan/vulkan.h>

#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"

#include "Game/Scenes/Menu/SavesData.hpp"

namespace Vox::Game::World
{
    class WorldManager
    {
        public:
            WorldManager(const Scenes::Menu::Saves::WorldData &wd);
            ~WorldManager();

        private:
            Chuncks::VoxelChunck *_firstChunck;
            Utils::Defines::dbuffer _chunckBuffer;

            /* private */
    
    };
} // namespace Vox::Game::World


#endif // __WORLDMANAGER_HPP__