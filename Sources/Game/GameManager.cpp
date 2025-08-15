#include "Game/GameManager.hpp"


namespace Vox::Game
{
    GameManager::GameManager()
    {
        this->_scManager.LoadScene("Menu");
    }
    
    GameManager::~GameManager()
    {
        
    }
}