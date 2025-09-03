#ifndef __GAMEMANAGER_HPP__
#define __GAMEMANAGER_HPP__

#include "Utils/Singleton.hpp"
#include "Utils/AUpdatable.hpp"

#include "Front/Scenes/ScenesManager.hpp"
#include "Game/InputManager.hpp"

#include "Game/Utils/TexturesData.hpp"

namespace Vox::Game
{
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
            void InitThreads();
            void CleanThreads();

        private:


            std::string _TPack = "1";
            Front::Scenes::ScenesManager _scManager;
            InputManager _iManager;
            std::unique_ptr<ThreadManager> _tManager;
            /* private */
    
    };
} // namespace Vox::Game


#endif // __GAMEMANAGER_HPP__