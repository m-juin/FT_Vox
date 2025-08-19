#include "Game/GameManager.hpp"


namespace Vox::Game
{
    void GameManager::Render()
    {
        this->_scManager.RenderCurrentScene();
        // std::cout << &this->_scManager << std::endl;
    }

    GameManager::GameManager()
    {
        this->_scManager.LoadScene("Menu");
        // std::cout << this->_scManager.GetCurrentScene().get() << std::endl;
    }
    
    GameManager::~GameManager()
    {
        
    }
}