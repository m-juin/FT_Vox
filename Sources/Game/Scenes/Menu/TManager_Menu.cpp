#include "Game/Scenes/Menu/TManager_Menu.hpp"

#include "Game/GameManager.hpp"

#include "Front/Rendering/Images/FontImage.hpp"

namespace Vox::Game::Scenes::Menu
{
    TManager_Menu::TManager_Menu()
    {
        this->CreateMap();
    }
    
    TManager_Menu::~TManager_Menu()
    {
        for (auto pair : this->_texturesMap)
            delete pair.second;
    }
    
    void TManager_Menu::CreateMap()
    {
        const std::string T_Path = Game::GameManager::GetInstance().GetTexturePackPath();
        this->_texturesMap["Menu_Main"] = new Front::Utils::TexturesAtlas(
            {
                {"Dirt", T_Path + "GUI/dirt.png"},
                {"Button", T_Path + "GUI/button.png"},
                {"Button_Hover", T_Path + "GUI/button_highlighted.png"},
                {"Button_Disabled", T_Path + "GUI/button_disabled.png"},
            },
            4
        );

        // this->_texturesMap["Menu_Buttons"] = new Front::Utils::TexturesAtlas(
        //     {
        //         {"Button", T_Path + "GUI/button.png"},
        //         {"Button_Hover", T_Path + "GUI/button_highlighted.png"},
        //     },
        //     4
        // );
    }
}