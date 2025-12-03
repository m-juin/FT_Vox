#include "Game/GameManager.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"
#include "Game/Scenes/World/Generation/SplinesManager.hpp"
#include "Game/Scenes/World/WorldManager.hpp"
#include "Game/ThreadManager.hpp"

#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/StaticGUIPipeline.hpp"

#include "Front/Scenes/TexturesManager.hpp"

#include "Utils/TracyUtils.hpp"

namespace Vox::Game
{
	void GameManager::Render()
	{
		this->_scManager.RenderCurrentScene();
	}

	GameManager::GameManager() : _wd({"", "", ""})
	{
		this->_tManager = std::make_unique<ThreadManager>(std::thread::hardware_concurrency() - 2);
		this->_sManager = std::make_unique<Generation::SplinesManager>(this->GetTexturePackPath());
		this->_texturesManager = std::make_unique<Front::Scenes::TexturesManager>(this->GetTexturePackPath());
		onUpdate.AddCallBack(
			[this]()
			{
				{
#ifdef TRACY_ENABLE
					ZoneScopedNC("Handle Per Frame Input", tracy::Color::BlueViolet);
#endif
					this->_iManager.HandlePerFrameInput();
				}
				{
#ifdef TRACY_ENABLE
					ZoneScopedNC("WorldManager Update", tracy::Color::Violet);
#endif
					if (this->_scManager.GetCurrentScene().GetName() == "World")
						World::WorldManager::GetInstance().Update();
				}
				{
#ifdef TRACY_ENABLE
					ZoneScopedNC("Interface Update", tracy::Color::DarkBlue);
#endif
					Vox::Front::Interfaces::InterfacesManager::GetInstance().Update();
				}
			});
	}

	void GameManager::InitGame()
	{
#ifdef DEBUG_WORLD
		Game::GameManager::GetInstance().SetSaveData({"1", "1", "1"});
		Game::GameManager::GetInstance().GetSceneManager().LoadScene("World");
		this->_scManager.ProcessSceneChange();
#else
		this->_scManager.LoadScene("Menu");
		this->_scManager.ProcessSceneChange();
#endif
	}

	GameManager::~GameManager() = default;

	void GameManager::InitThreads() const
	{
		if (!this->_tManager)
			return;
		this->_tManager->BuildPool(*this->_sManager, *this->_texturesManager);
	}

	void GameManager::CleanThreads() const
	{
		if (!this->_tManager)
			return;
		this->_tManager->CleanPool();
	}
} // namespace Vox::Game
