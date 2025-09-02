#include "Game/Scenes/World/Sc_World.hpp"

#include "Game/Scenes/World/TManager_World.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"

#include "Game/Scenes/Menu/SavesData.hpp"
// #include "Front/Rendering/SwapChain.hpp"

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
		delete this->_textureManager;
    }

	void Sc_World::Render()
	{
		if (this->_wM)
			this->_wM->Render();
	}

	void Sc_World::InitSceneData()
    {
		auto &iManager = Front::Interfaces::InterfacesManager::GetInstance();
        (void)iManager;
		this->_textureManager = new Scenes::World::TManager_World();
		this->_wM = std::make_unique<Game::World::WorldManager>(Menu::Saves::WorldData("", "", ""));
    }
} // namespace Vox::Game::Scenes::World

template <> void Vox::Front::Scenes::RegisterSceneFactory<Vox::Game::Scenes::World::Sc_World>()
{
	static_assert(std::is_base_of_v<AScene, Vox::Game::Scenes::World::Sc_World>, "Sc_World doit hériter de AScene");
	g_sceneFactories[Vox::Game::Scenes::World::Sc_World::getName()] = []() { return std::make_unique<Vox::Game::Scenes::World::Sc_World>(); };
}