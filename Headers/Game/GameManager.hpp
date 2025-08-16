#ifndef __GAMEMANAGER_HPP__
#define __GAMEMANAGER_HPP__

#include "Utils/Singleton.hpp"

#include "Front/Scenes/ScenesManager.hpp"

namespace Vox::Game
{
    class GameManager : public Utils::Singleton<GameManager>
    {
        friend class Utils::Singleton<GameManager>;

        public:
            GameManager();
            ~GameManager();

            void Render();

            Front::Scenes::ScenesManager &GetSceneManager() {return this->_scManager;};
        private:
            Front::Scenes::ScenesManager _scManager;
            /* private */
    
    };
} // namespace Vox::Game


#endif // __GAMEMANAGER_HPP__