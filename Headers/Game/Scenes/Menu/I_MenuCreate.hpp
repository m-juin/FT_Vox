#ifndef __I_MENUCREATE_HPP__
#define __I_MENUCREATE_HPP__

#include "Front/Interfaces/AInterface.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
    class I_MenuCreate : public virtual Front::Interfaces::AInterface
    {
        public:
            I_MenuCreate(Vox::Front::Interfaces::Elements::Vector2 pos = {0.f, 0.f}, Vox::Front::Interfaces::Elements::Vector2 size = {0.f, 0.f});
            ~I_MenuCreate() {};
    
			void SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos) override;
			void SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize) override;
            // void OnHover() override {};
            // void OnHoverLeave() override {};
            void Render() override;

        private:
            /* private */
    
    };
}

#endif // __I_MENUCREATE_HPP__