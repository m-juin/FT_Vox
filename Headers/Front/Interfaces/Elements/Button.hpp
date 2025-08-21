#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include "./Bases/AElement.hpp"

namespace Vox::Front::Interfaces::Elements
{
    class Text;
    class Image;

    class Button : public Bases::AElement
    {
        public:
            Button();
            ~Button();
    
        private:
            /* private */
    
    };
}

#endif // __BUTTON_HPP__