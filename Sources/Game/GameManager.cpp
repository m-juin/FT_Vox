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
        onUpdate.AddCallBack([this]()
    {
        this->_iManager.HandlePerFrameInput();
    });
    }

    void GameManager::InitGame()
    {
        this->_scManager.LoadScene("Menu");
        this->_scManager.ProcessSceneChange();
    }

    GameManager::~GameManager()
    {
        
    }
}