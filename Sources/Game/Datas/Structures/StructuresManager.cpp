#include "Game/Datas/Structures/StructuresManager.hpp"

#include "Game/GameManager.hpp"

namespace Vox::Game::Datas::Structures
{
    StructuresManager::StructuresManager()
    {
        LoadStructures();
    }
    
    StructuresManager::~StructuresManager()
    {
        
    }
    
    std::unordered_map<MGL::Vectors::Vector3<uint8_t>, Blocks::BlockType, MGL::Vectors::Vector3Hash<uint8_t>> StructuresManager::GetStructBlocks(StructuresType type)
    {
        (void)type;
        return {};
    }
    
    void StructuresManager::LoadStructures()
    {
        const std::string Tpath = Game::GameManager::GetInstance().GetTexturePackPath();
        // std::pair<StructuresType, Structure> pair = {}
        this->_list.insert_or_assign(StructuresType::Oak_Tree1, Structure(Tpath + "Structures/Trees/Oak1.st"));
    }
}