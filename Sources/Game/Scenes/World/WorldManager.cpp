#include "Game/Scenes/World/WorldManager.hpp"


namespace Vox::Game::World
{
    WorldManager::WorldManager(const Scenes::Menu::Saves::WorldData &wd) : _chunckBuffer(2, Utils::Vulkan::GetAlignedChunckSize() * Utils::Defines::CHUNCK_AMOUNT, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
    {
        this->_firstChunck = new Chuncks::VoxelChunck(0);
    }
    
    WorldManager::~WorldManager()
    {
        if (this->_firstChunck)
            delete this->_firstChunck;
    }
}