#ifndef __WORLDMANAGER_HPP__
#define __WORLDMANAGER_HPP__

#include "Game/Scenes/World/Utils/Defines.hpp"
#include "Game/Scenes/World/Utils/Vulkan.hpp"

#include <array>
#include <vulkan/vulkan.h>

#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"

#include "Game/Scenes/Menu/SavesData.hpp"

#include "Game/GameManager.hpp"
#include "Game/Scenes/World/Sc_World.hpp"
#include "Game/Scenes/World/Player/Camera.hpp"
#include "Front/Scenes/ScenesManager.hpp"

namespace Vox::Game::Scenes::World
{
    class Sc_World;
}

namespace Vox::Game::World
{
    class WorldManager
    {
        public:
            WorldManager() = delete;
            WorldManager(const Scenes::Menu::Saves::WorldData &wd);
            ~WorldManager();

        static WorldManager &GetInstance();

        void UpdateBuffer(const size_t &index, const Chuncks::VoxelChunck::ChunckUniform &uniform);

        void Render();

        private:
            Chuncks::VoxelChunck *_firstChunck;
            Scenes::World::Player::Camera camera;
            Utils::Defines::dbuffer _chunckBuffer;

            /* private */
    
    };
} // namespace Vox::Game::World


#endif // __WORLDMANAGER_HPP__