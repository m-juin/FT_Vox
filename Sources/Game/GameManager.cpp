#include "Game/GameManager.hpp"


namespace Vox::Game
{
    GameManager::GameManager()
    {
        std::cout << "here" << std::endl;
        this->_scManager.LoadScene("Menu");
    }
    
    GameManager::~GameManager()
    {
        
    }
}