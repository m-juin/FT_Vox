#include "Game/Datas/Structures/StructuresManager.hpp"

namespace Vox::Game::Datas::Structures
{
    StructuresManager::StructuresManager()
    {
        LoadStructures();
    }
    
    StructuresManager::~StructuresManager()
    {
        
    }
    
    std::unordered_map<MGL::Vectors::Vector3<uint8_t>, Blocks::BlockType, MGL::Vectors::Vector3Hash<uint8_t>> StructuresManager::GetStructBlocks(StructuresType _type)
    {
        
    }
    
    void StructuresManager::LoadStructures()
    {
        
    }
}