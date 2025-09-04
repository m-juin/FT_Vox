#ifndef __GAMEMANAGER_HPP__
#define __GAMEMANAGER_HPP__

#include "Utils/Singleton.hpp"
#include "Utils/AUpdatable.hpp"

#include "Front/Scenes/ScenesManager.hpp"
#include "Game/InputManager.hpp"

#include "Game/Utils/TexturesData.hpp"
#include "Game/Scenes/World/Generation/SplinesManager.hpp"

namespace Vox::Game
{
    namespace Generation
    {
        class SplinesManager;
    } // namespace Generation
    
    class ThreadManager;
    class GameManager : public Vox::Utils::Singleton<GameManager>, public Vox::Utils::AUpdatable
    {
        friend class Vox::Utils::Singleton<GameManager>;

        public:
            GameManager();
            ~GameManager();

            void InitGame();
            void Render();

            Front::Scenes::ScenesManager &GetSceneManager() {return this->_scManager;};
            const Front::Scenes::ScenesManager &GetSceneManager() const {return this->_scManager;};
            InputManager &GetInputManager() {return this->_iManager;};

            const std::string GetTexturePackPath() {return Game::Utils::Textures::TPacks_Path + this->_TPack + "/";}
            void InitThreads() const;
            void CleanThreads() const;
            ThreadManager &GetThreadManager() {return *_tManager;};

        private:

            std::unique_ptr<Generation::SplinesManager> _sManager;
            std::string _TPack = "1";
            Front::Scenes::ScenesManager _scManager;
            InputManager _iManager;
            std::unique_ptr<ThreadManager> _tManager;
            /* private */
    
    };
} // namespace Vox::Game


#endif // __GAMEMANAGER_HPP__