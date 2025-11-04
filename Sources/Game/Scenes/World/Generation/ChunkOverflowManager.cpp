#include "Game/Scenes/World/Generation/ChunkOverflowManager.hpp"


namespace Vox::Game::Generation
{
    void ChunkOverflowManager::AddBlock(const Vector2Int &clusterPos, ChunkOverflowBlock block)
    {
        std::lock_guard<std::mutex> lock(this->_mut);
        this->_map[clusterPos].push_back(block);
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