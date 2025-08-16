#ifndef __SCENESMANAGER_HPP__
#define __SCENESMANAGER_HPP__

#include "AScene.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

namespace Vox::Front::Scenes
{
    using SceneCreator = std::function<std::unique_ptr<AScene>()>;

    extern std::unordered_map<std::string, SceneCreator> g_sceneFactories;

    template<typename T>
    void RegisterSceneFactory()
    {
        static_assert(std::is_base_of_v<AScene, T>, "T doit hériter de AScene");
        g_sceneFactories[T::getName()] = []() {
            return std::make_unique<T>();
        };
    }

    class ScenesManager
    {
    public:
        void RenderCurrentScene();
        ScenesManager();
        ~ScenesManager();
        bool LoadScene(const std::string& sceneName);
    private:
        std::unique_ptr<AScene> _currentScene;
    };
} // namespace Vox::Front::Scenes

#endif // __SCENESMANAGER_HPP__