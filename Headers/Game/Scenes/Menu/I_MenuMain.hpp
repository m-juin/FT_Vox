#ifndef __I_MENUMAIN_HPP__
#define __I_MENUMAIN_HPP__

#include "Front/Interfaces/AInterface.hpp"

#include "MathGraphicalLib/Vectors/Vector2.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
    using Vector2 = MGL::Vectors::Vector2<size_t> ;
    class I_MenuMain : public virtual Front::Interfaces::AInterface
    {
        public:
            I_MenuMain(Vector2 pos = {0, 0}, Vector2 size = {0, 0});
            ~I_MenuMain();
    
			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;
            void OnHover() override {};
            void OnHoverLeave() override {};
            void Render() override;

        private:
            /* private */
    
    };
}

#endif // __MENUMAIN_HPP__