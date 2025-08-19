#ifndef __TMANAGER_MENU_HPP__
#define __TMANAGER_MENU_HPP__

#include "Front/Scenes/ATexturesManager.hpp"

namespace Vox::Game::Scenes::Menu
{
    class TManager_Menu : public Front::Scenes::ATexturesManager
    {
        public:
            TManager_Menu();
            ~TManager_Menu();
            void CreateMap() override;
        private:
            /* private */
    
    };
} // namespace Vox::Game::Scenes::Menu


#endif // __TMANAGER_MENU_HPP__