#include "Game/GameManager.hpp"


namespace Vox::Game
{
    void GameManager::Render()
    {
        this->_scManager.RenderCurrentScene();
    }

    GameManager::GameManager()
    {
        std::cout << "here" << std::endl;
        this->_scManager.LoadScene("Menu");
    }
    
    GameManager::~GameManager()
    {
        
    }
}