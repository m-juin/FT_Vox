#include "Game/Scenes/Menu/Sc_Menu.hpp"
#include "Game/Scenes/Menu/I_MenuMain.hpp"
#include <iostream>

#include "Front/Interfaces/InterfacesManager.hpp"

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
		// Implémentation du déchargement du menu
	}

	void Sc_Menu::Render()
	{
		std::cout << "Dessin du menu..." << std::endl;
		auto &iManager = Front::Interfaces::InterfacesManager::GetInstance();
		iManager.Render();
		// Implémentation du dessin du menu
	}
	
	void Sc_Menu::InitSceneData()
	{
		auto &iManager = Front::Interfaces::InterfacesManager::GetInstance();
		iManager.RegisterInterface(new Interfaces::I_MenuMain());
	}

	

} // namespace Vox::Game::Scenes::Menu

template <> void Vox::Front::Scenes::RegisterSceneFactory<Vox::Game::Scenes::Menu::Sc_Menu>()
{
	static_assert(std::is_base_of_v<AScene, Vox::Game::Scenes::Menu::Sc_Menu>, "Sc_Menu doit hériter de AScene");
	g_sceneFactories[Vox::Game::Scenes::Menu::Sc_Menu::getName()] = []() { return std::make_unique<Vox::Game::Scenes::Menu::Sc_Menu>(); };
}