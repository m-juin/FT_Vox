#ifndef __MENU_HPP__
#define __MENU_HPP__

#include "AInterface.hpp"

namespace Vox::Front::Interfaces
{
    class Menu : public AInterface
    {
        public:
            Menu(Vector2 pos = {0, 0}, Vector2 size = {0, 0});
            ~Menu();
    
			void SetPos(const Vector2 newPos);
			void SetSize(const Vector2 newSize);

        private:
            /* private */
    
    };
}

#endif // __MENU_HPP__