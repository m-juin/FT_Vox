#include "Game/Scenes/World/Generation/ChunkOverflowManager.hpp"

#include "Game/Scenes/World/WorldManager.hpp"
#include "Game/Scenes/World/Generation/GenerationManager.hpp"

namespace Vox::Game::Generation
{
    void ChunkOverflowManager::AddBlock(const Vector2Int &clusterPos, ChunkOverflowBlock block)
    {
        auto cluster = World::WorldManager::GetInstance().GetCluster(clusterPos);
        if (cluster == nullptr)
        {
            std::lock_guard<std::mutex> lock(this->_mut);
            this->_map[clusterPos].push_back(block);
        }
        // else
            // cluster->SetBlock({static_cast<uint8_t>(block.localCoord[0]), static_cast<uint8_t>(block.localCoord[1]), static_cast<uint8_t>(block.localCoord[2])}, block.type);
    }
    
    std::vector<ChunkOverflowBlock> ChunkOverflowManager::ExtractClusterBlocks(const Vector2Int &clusterPos)
    {
        std::lock_guard<std::mutex> lock(this->_mut);
        auto it = this->_map.find(clusterPos);
        if (it == _map.end())
            return {};
        auto vec = std::move(it->second);
        this->_map.erase(it);
        return vec;
    }
    
    ChunkOverflowManager::ChunkOverflowManager()
    {
        
    }
}