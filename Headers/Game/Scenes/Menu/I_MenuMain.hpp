#ifndef __I_MENUMAIN_HPP__
#define __I_MENUMAIN_HPP__

#include "Front/Interfaces/AInterface.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
    class I_MenuMain : public virtual Front::Interfaces::AInterface
    {
        public:
            I_MenuMain(Vox::Front::Interfaces::Elements::Vector2Float pos = {0.f, 0.f}, Vox::Front::Interfaces::Elements::Vector2Float size = {0.f, 0.f});
            ~I_MenuMain();
    
			void SetPos(const Vox::Front::Interfaces::Elements::Vector2Float newPos) override;
			void SetSize(const Vox::Front::Interfaces::Elements::Vector2Float newSize) override;
            void OnHover() override {};
            void OnHoverLeave() override {};
            void Render() override;

        private:
            /* private */
    
    };
}

#endif // __MENUMAIN_HPP__