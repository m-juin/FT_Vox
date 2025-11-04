#ifndef __CHUNKOVERFLOWMANAGER_HPP__
#define __CHUNKOVERFLOWMANAGER_HPP__

#include "MathGraphicalLib/Vectors/Vector3.hpp"
#include "MathGraphicalLib/Vectors/Vector2.hpp"

#include "Game/Datas/Blocks/BlocksType.hpp"

#include <unordered_map>
#include <vector>
#include <mutex>

namespace Vox::Game::Generation
{
    using Vector3Int = MGL::Vectors::Vector3<int>;
    using Vector2Int = MGL::Vectors::Vector2<int>;
    using BlockType = Game::Datas::Blocks::BlockType;

    struct ChunkOverflowBlock
    {
        Vector3Int localCoord;
        BlockType type;
    };
    

    class ChunkOverflowManager
    {
        public:
            void AddBlock(const Vector2Int &clusterPos, ChunkOverflowBlock block);
            std::vector<ChunkOverflowBlock> ExtractClusterBlocks(const Vector2Int &clusterPos);
            ChunkOverflowManager();
            ~ChunkOverflowManager() {};
    
        private:
            std::unordered_map<Vector2Int, std::vector<ChunkOverflowBlock>, MGL::Vectors::Vector2Hash<int>> _map;
            std::mutex _mut;
            /* private */
    
    };
} // namespace Vox::Game::Generation


#endif // __CHUNKOVERFLOWMANAGER_HPP__