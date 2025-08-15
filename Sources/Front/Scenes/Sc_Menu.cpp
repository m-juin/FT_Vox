#include "Sc_Menu.hpp"
#include <iostream>

namespace Vox::Front::Scenes
{
    void Sc_Menu::Load()
    {
        std::cout << "Chargement du menu..." << std::endl;
        // Implémentation du chargement du menu
    }
    
    void Sc_Menu::Unload()
    {
        std::cout << "Déchargement du menu..." << std::endl;
        // Implémentation du déchargement du menu
    }
    
    void Sc_Menu::Draw()
    {
        std::cout << "Dessin du menu..." << std::endl;
        // Implémentation du dessin du menu
    }
    
    template <>
    void RegisterSceneFactory<Sc_Menu>()
    {
        static_assert(std::is_base_of_v<AScene, Sc_Menu>, "Sc_Menu doit hériter de AScene");
        g_sceneFactories[Sc_Menu::getName()] = [](const std::string& name) {
            return std::make_unique<Sc_Menu>(name);
        };
    }
} // namespace Vox::Front::Scenes