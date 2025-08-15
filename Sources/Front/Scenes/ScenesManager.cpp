#include "ScenesManager.hpp"

namespace Vox::Front::Scenes
{
    std::unordered_map<std::string, SceneCreator> g_sceneFactories;

    ScenesManager::ScenesManager() : _currentScene(nullptr) {}
    
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
        
        auto factoryIt = g_sceneFactories.find(sceneName);
        if (factoryIt != g_sceneFactories.end())
        {
            _currentScene = factoryIt->second(sceneName);
            if (_currentScene)
            {
                _currentScene->Load();
                return true;
            }
        }
        return false;
    }
} // namespace Vox::Front::Scenes