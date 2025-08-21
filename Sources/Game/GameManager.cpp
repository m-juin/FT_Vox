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
        // std::cout << this->_scManager.GetCurrentScene().get() << std::endl;
    }
    
    void GameManager::InitGame()
    {
        this->_scManager.LoadScene("Menu");
    }

    GameManager::~GameManager()
    {
        
    }
}