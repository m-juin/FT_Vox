#include "Game/Scenes/Menu/TManager_Menu.hpp"

#include "Game/GameManager.hpp"

namespace Vox::Game::Scenes::Menu
{
    TManager_Menu::TManager_Menu()
    {
        this->CreateMap();
    }
    
    TManager_Menu::~TManager_Menu()
    {
        for (auto pair : this->_atlasMap)
            delete pair.second;
    }
    
    void TManager_Menu::CreateMap()
    {
        const std::string T_Path = Game::GameManager::GetInstance().GetTexturePackPath();
        this->_atlasMap["Menu_Main"] = new Front::Utils::TexturesAtlas(
            {
                {"BlackStone", T_Path + "GUI/blackstone.png"},
                {"Stone", T_Path + "GUI/stone.png"},
                {"CobbleStone", T_Path +"cobblestone.png"},
            },
            16,
            4
        );
    }
}