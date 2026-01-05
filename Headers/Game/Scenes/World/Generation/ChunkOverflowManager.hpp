#ifndef __CHUNKOVERFLOWMANAGER_HPP__
#define __CHUNKOVERFLOWMANAGER_HPP__

#include "MathGraphicalLib/Vectors/Defines.hpp"

#include "Game/Datas/Blocks/BlocksType.hpp"

#include <unordered_map>
#include <vector>
#include <mutex>

namespace Vox::Game::Generation
{
    using namespace MGL::Vectors::Types;
    using BlockType = Game::Datas::Blocks::BlockType;

    struct ChunkOverflowBlock
    {
        Vector3Int worldCoord;
        BlockType type;
    };
    

    class ChunkOverflowManager
    {
        public:
            void AddBlock(const Vector2Int &, ChunkOverflowBlock);
            void AddBlocks(const Vector2Int &, std::vector<ChunkOverflowBlock>);
            std::vector<ChunkOverflowBlock> ExtractClusterBlocks(const Vector2Int &);
            ChunkOverflowManager();
            ~ChunkOverflowManager() {};
    
        private:
            std::unordered_map<Vector2Int, std::vector<ChunkOverflowBlock>, MGL::Vectors::Vector2Hash<int>> _map;
            std::mutex _mut;
            /* private */
    
    };
} // namespace Vox::Game::Generation


#endif // __CHUNKOVERFLOWMANAGER_HPP__