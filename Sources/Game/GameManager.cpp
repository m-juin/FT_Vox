#include "Game/GameManager.hpp"

#include "Game/ThreadManager.hpp"
#include "Game/Scenes/World/WorldManager.hpp"
#include "Game/Scenes/World/Generation/SplinesManager.hpp"
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
		this->_sManager = std::make_unique<Generation::SplinesManager>(this->GetTexturePackPath());
		this->_texturesManager = std::make_unique<Front::Scenes::TexturesManager>(this->GetTexturePackPath());

		onUpdate.AddCallBack([this]()
		{
			this->_iManager.HandlePerFrameInput();
			if (this->_scManager.GetCurrentScene().GetName() == "World")
				World::WorldManager::GetInstance().Update();
		});
	}

	void GameManager::InitGame()
	{
		this->_scManager.LoadScene("Menu");
		this->_scManager.ProcessSceneChange();
	}

	GameManager::~GameManager() = default;

	void GameManager::InitThreads() const
	{
		if (!this->_tManager)
			return;
		std::cout << "init threads\n";
		this->_tManager->BuildPool(*this->_sManager);
	}

	void GameManager::CleanThreads() const
	{
		if (!this->_tManager)
			return;
		this->_tManager->CleanPool();
	}
} // namespace Vox::Game
