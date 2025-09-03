#include "Game/GameManager.hpp"

#include "Game/ThreadManager.hpp"

namespace Vox::Game
{

	void GameManager::Render()
	{
		this->_scManager.RenderCurrentScene();
		// std::cout << &this->_scManager << std::endl;
	}

	GameManager::GameManager()
	{
        this->_tManager = std::make_unique<ThreadManager>(std::thread::hardware_concurrency() - 2);
		// std::cout << this->_scManager.GetCurrentScene().get() << std::endl;
		onUpdate.AddCallBack([this]() { this->_iManager.HandlePerFrameInput(); });
	}

	void GameManager::InitGame()
	{
		this->_scManager.LoadScene("Menu");
		this->_scManager.ProcessSceneChange();
	}

	GameManager::~GameManager() {}

	void GameManager::InitThreads()
    {
        if (!this->_tManager)
            return ;
        std::cout << "init threads\n";
        this->_tManager->BuildPool();
    }
    
    void GameManager::CleanThreads()
    {
        if (!this->_tManager)
            return ;
        this->_tManager->CleanPool();
    }
} // namespace Vox::Game