#include "Game/Scenes/World/Sc_World.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"

#include "Front/Rendering/SwapChain.hpp"
#include "Game/Scenes/Menu/SavesData.hpp"
#include "Game/Scenes/World/Interfaces/I_F3.hpp"
#include "Game/Scenes/World/Interfaces/I_Generation.hpp"

namespace Vox::Game::Scenes::World
{
	void Sc_World::Load()
	{
		std::cout << "Chargement du monde..." << std::endl;
		InitSceneData();
	}

	void Sc_World::Unload()
	{
		std::cout << "Déchargement du monde..." << std::endl;
		Front::Interfaces::InterfacesManager::GetInstance().ResetInterfacesList();
		GameManager::GetInstance().CleanThreads();
	}

	void Sc_World::Render()
	{
		if (this->_wM)
			this->_wM->Render();
		auto &iManager = Front::Interfaces::InterfacesManager::GetInstance();
		iManager.Render();
	}

	void Sc_World::InitSceneData()
	{
		auto &iManager = Front::Interfaces::InterfacesManager::GetInstance();
		GameManager::GetInstance().GetInputManager().SetInputTarget(E_InputTarget::Camera);

		auto extent = Front::Rendering::SwapChain::GetInstance().GetExtent();
		iManager.RegisterInterface("F3", new Interfaces::I_F3({0, 0}, {(float)extent.width, (float)extent.height}));
		iManager.RegisterInterface("Generation",
								   new Interfaces::I_Generation({0, 0}, {(float)extent.width, (float)extent.height}));
		GameManager::GetInstance().InitThreads();
		this->_wM = std::make_unique<Game::World::WorldManager>(GameManager::GetInstance().GetSaveData());
	}

	bool Sc_World::HandleInputAction(const Game::Utils::Datas::E_InputAction &action)
	{
		auto &iManager = Front::Interfaces::InterfacesManager::GetInstance();
		switch (action)
		{
		case Game::Utils::Datas::E_InputAction::I_F3:
			if (iManager.IsInterfaceEnabled("F3") == true)
			{
				iManager.DisableInterface("F3");
			}
			else
			{
				iManager.EnableInterface("F3");
				iManager.DisableInterface("Generation");
			}
			break;
		case Game::Utils::Datas::E_InputAction::I_Generation:
			if (iManager.IsInterfaceEnabled("Generation"))
			{
				iManager.DisableInterface("Generation");
			}
			else
			{
				iManager.EnableInterface("Generation");
				iManager.DisableInterface("F3");
				return true;
			}
			break;

		default:
			break;
		}
		return false;
	}
} // namespace Vox::Game::Scenes::World

template <> void Vox::Front::Scenes::RegisterSceneFactory<Vox::Game::Scenes::World::Sc_World>()
{
	static_assert(std::is_base_of_v<AScene, Vox::Game::Scenes::World::Sc_World>, "Sc_World doit hériter de AScene");
	g_sceneFactories[Vox::Game::Scenes::World::Sc_World::getName()] = []()
	{ return std::make_unique<Vox::Game::Scenes::World::Sc_World>(); };
}