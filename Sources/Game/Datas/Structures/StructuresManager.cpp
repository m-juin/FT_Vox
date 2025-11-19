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
    
    void StructuresManager::LoadStructures()
    {
        const std::string Tpath = Game::GameManager::GetInstance().GetTexturePackPath();
        this->_list.insert_or_assign(StructuresType::Oak_Tree1, Structure(Tpath + "Structures/Trees/Oak1.st"));
        this->_list.insert_or_assign(StructuresType::Oak_Tree2, Structure(Tpath + "Structures/Trees/Oak2.st"));
    }
}