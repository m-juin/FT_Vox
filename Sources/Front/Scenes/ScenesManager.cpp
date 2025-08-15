#include "Front/Scenes/ScenesManager.hpp"
#include "Front/Scenes/Sc_Menu.hpp"
#include <iostream>

namespace Vox::Front::Scenes
{
    std::unordered_map<std::string, SceneCreator> g_sceneFactories;

    ScenesManager::ScenesManager() : _currentScene(nullptr)
    {
        RegisterSceneFactory<Sc_Menu>();
    }
    
    ScenesManager::~ScenesManager()
    {
        if (_currentScene)
        {
            _currentScene->Unload();
            _currentScene.reset();
        }
    }
    
    bool ScenesManager::LoadScene(const std::string& sceneName)
    {
        if (_currentScene)
        {
            _currentScene->Unload();
            _currentScene.reset();
        }
        

        std::cout << "Trying to load : " << sceneName << "\n\nList of avalaible scenes:\n" << std::endl;
        for (auto key : g_sceneFactories)
        {
            std::cout << "\t" << key.first << std::endl;
        }
        auto factoryIt = g_sceneFactories.find(sceneName);
        if (factoryIt != g_sceneFactories.end())
        {
            _currentScene = factoryIt->second();
            if (_currentScene)
            {
                _currentScene->Load();
                return true;
            }
        }
        return false;
    }
} // namespace Vox::Front::Scenes