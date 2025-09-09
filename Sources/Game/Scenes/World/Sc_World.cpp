#include "Game/Scenes/World/Sc_World.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"

#include "Game/Scenes/Menu/SavesData.hpp"
#include "Game/Scenes/World/Interfaces/I_F3.hpp"
#include "Game/Scenes/World/Interfaces/I_Generation.hpp"
#include "Front/Rendering/SwapChain.hpp"

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
		iManager.RegisterInterface("Generation", new Interfaces::I_Generation({0, 0}, {(float)extent.width, (float)extent.height}));
		GameManager::GetInstance().InitThreads();
		this->_wM = std::make_unique<Game::World::WorldManager>(GameManager::GetInstance().GetSaveData());
    }
} // namespace Vox::Game::Scenes::World

template <> void Vox::Front::Scenes::RegisterSceneFactory<Vox::Game::Scenes::World::Sc_World>()
{
	static_assert(std::is_base_of_v<AScene, Vox::Game::Scenes::World::Sc_World>, "Sc_World doit hériter de AScene");
	g_sceneFactories[Vox::Game::Scenes::World::Sc_World::getName()] = []() { return std::make_unique<Vox::Game::Scenes::World::Sc_World>(); };
}