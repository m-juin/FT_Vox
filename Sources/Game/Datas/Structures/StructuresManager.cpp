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
    
    // const std::pair(std::vector<Blocks::BlockType>, MGL::Vectors::Vector3) &StructuresManager::GetStructBlocks(StructuresType type) const
    // {
    //     auto it = this->_list.find(type);
    //     if (it == this->_list.end())
    //         throw std::runtime_error("Structure type not found");
    //     return it->second.GetContent();
    //     return {};
    // }
    
    void StructuresManager::LoadStructures()
    {
        const std::string Tpath = Game::GameManager::GetInstance().GetTexturePackPath();
        // std::pair<StructuresType, Structure> pair = {}
        this->_list.insert_or_assign(StructuresType::Oak_Tree1, Structure(Tpath + "Structures/Trees/Oak1.st"));
    }
}