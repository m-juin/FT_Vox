#ifndef __I_MENUMain_HPP__
#define __I_MENUMain_HPP__

#include "Front/Interfaces/AInterface.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
    class I_MenuMain : public Front::Interfaces::AInterface
    {
        public:
            I_MenuMain(Vector2 pos = {0, 0}, Vector2 size = {0, 0});
            ~I_MenuMain();
    
			void SetPos(const Vector2 newPos);
			void SetSize(const Vector2 newSize);

        private:
            /* private */
    
    };
}

#endif // __MENU_HPP__