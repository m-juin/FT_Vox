#include "Game/Scenes/Menu/TManager_Menu.hpp"


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
        this->_atlasMap["Menu_Main"] = new Front::Utils::TexturesAtlas(
            {
                {"BlackStone", "Assets/TexturesPack/1/GUI/blackstone.png"},
                {"Stone", "Assets/TexturesPack/1/GUI/stone.png"},
                {"CobbleStone", "Assets/TexturesPack/1/GUI/cobblestone.png"},
            },
            16,
            4
        );
    }
}