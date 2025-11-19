#include "Game/Scenes/World/Generation/ChunkOverflowManager.hpp"

#include "Game/Scenes/World/Generation/GenerationManager.hpp"
#include "Game/Scenes/World/WorldManager.hpp"

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
	}

	void ChunkOverflowManager::AddBlocks(const Vector2Int &clusterPos, std::vector<ChunkOverflowBlock> blocks)
	{
		std::lock_guard<std::mutex> lock(this->_mut);
		auto &vec = this->_map[clusterPos];
		vec.insert(vec.end(), blocks.begin(), blocks.end());
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

	ChunkOverflowManager::ChunkOverflowManager() {}
} // namespace Vox::Game::Generation