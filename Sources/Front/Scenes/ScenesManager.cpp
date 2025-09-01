#include "Front/Scenes/ScenesManager.hpp"
#include "Game/Scenes/Menu/Sc_Menu.hpp"
#include "Game/Scenes/World/Sc_World.hpp"
#include <iostream>

#include "Front/Interfaces/InterfacesManager.hpp"

namespace Vox::Front::Scenes
{
    void ScenesManager::RenderCurrentScene()
    {
        if (this->_currentScene)
        {
            this->_currentScene->Render();
        }
    }

    std::unordered_map<std::string, SceneCreator> g_sceneFactories;

    ScenesManager::ScenesManager() : _currentScene(nullptr)
    {
        RegisterSceneFactory<Game::Scenes::Menu::Sc_Menu>();
        RegisterSceneFactory<Game::Scenes::World::Sc_World>();
    }
    
    ScenesManager::~ScenesManager()
    {
        if (_currentScene)
        {
            _currentScene->Unload();
            _currentScene.reset();
        }
    }
    
    void ScenesManager::LoadScene(const std::string& sceneName)
    {
        _pendingScene = sceneName;
    }

    bool ScenesManager::ProcessSceneChange()
    {
        if (this->_pendingScene.size() == 0) return false ;
        if (_currentScene)
        {
            _currentScene->Unload();
            Front::Interfaces::InterfacesManager::GetInstance().ResetInterfacesList();
            _currentScene.reset();
        }
        
        auto factoryIt = g_sceneFactories.find(this->_pendingScene);
        if (factoryIt != g_sceneFactories.end())
        {
            _currentScene = factoryIt->second();
            if (_currentScene)
            {
                _currentScene->Load();
                this->_pendingScene = "";
                return true;
            }
        }
        this->_pendingScene = "";
        return false;

    }

} // namespace Vox::Front::Scenes