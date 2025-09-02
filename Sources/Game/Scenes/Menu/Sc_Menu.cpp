#include "Game/Scenes/Menu/Sc_Menu.hpp"
#include "Game/Scenes/Menu/I_MenuMain.hpp"
#include "Game/Scenes/Menu/I_MenuWorld.hpp"
#include "Game/Scenes/Menu/I_MenuCreate.hpp"
#include "Game/Scenes/Menu/TManager_Menu.hpp"

#include "Game/GameManager.hpp"
#include <iostream>

#include "Front/Interfaces/InterfacesManager.hpp"

#include "Front/Rendering/SwapChain.hpp"

#include <filesystem>

namespace Vox::Game::Scenes::Menu
{
	void Sc_Menu::Load()
	{
		std::cout << "Chargement du menu..." << std::endl;
		InitSceneData();
		// Implémentation du chargement du menu
	}

	void Sc_Menu::Unload()
	{
		std::cout << "Déchargement du menu..." << std::endl;
		Front::Interfaces::InterfacesManager::GetInstance().ResetInterfacesList();
		delete this->_textureManager;
		// Implémentation du déchargement du menu
	}

	void Sc_Menu::Render()
	{
		// std::cout << "Dessin du menu..." << std::endl;
		auto &iManager = Front::Interfaces::InterfacesManager::GetInstance();
		iManager.Render();
		// Implémentation du dessin du menu
	}
	
	void Sc_Menu::InitSceneData()
	{	
		auto &iManager = Front::Interfaces::InterfacesManager::GetInstance();
		auto extent = Front::Rendering::SwapChain::GetInstance().GetExtent();
		GameManager::GetInstance().GetInputManager().SetInputTarget(E_InputTarget::UI);
		this->_textureManager = new Scenes::Menu::TManager_Menu();

		iManager.RegisterInterface("Main", new Interfaces::I_MenuMain({0, 0}, {(float)extent.width, (float)extent.height}));
		iManager.RegisterInterface("World", new Interfaces::I_MenuWorld({0, 0}, {(float)extent.width, (float)extent.height}));
		iManager.RegisterInterface("Create", new Interfaces::I_MenuCreate({0, 0}, {(float)extent.width, (float)extent.height}));
	}

} // namespace Vox::Game::Scenes::Menu

template <> void Vox::Front::Scenes::RegisterSceneFactory<Vox::Game::Scenes::Menu::Sc_Menu>()
{
	static_assert(std::is_base_of_v<AScene, Vox::Game::Scenes::Menu::Sc_Menu>, "Sc_Menu doit hériter de AScene");
	g_sceneFactories[Vox::Game::Scenes::Menu::Sc_Menu::getName()] = []() { return std::make_unique<Vox::Game::Scenes::Menu::Sc_Menu>(); };
}