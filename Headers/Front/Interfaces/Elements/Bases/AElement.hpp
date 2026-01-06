#ifndef __AELEMENT_HPP__
#define __AELEMENT_HPP__

#include <cstdint>

#include "../Utils/Defines.hpp"
#include "MathGraphicalLib/Vectors/Defines.hpp"

namespace Vox::Front::Interfaces::Elements
{
    using namespace Vox::Front::Interfaces::Elements::Utils::Defines;
}

namespace Vox::Front::Interfaces::Elements::Bases
{
    using namespace MGL::Vectors::Types;
    class AElement
    {
        public:
            virtual void Draw() = 0;
            virtual ~AElement() {};
            virtual void SetPos(const Vector2Float newPos) = 0;
            virtual void SetSize(const Vector2Float newSize) = 0;
            virtual void ResetVertex() = 0;

            Vector2Float GetPos() {return this->_pos;};
            Vector2Float GetSize() {return this->_size;};
        protected:
            Vector2Float _pos;
            Vector2Float _size;
            AElement(Vector2Float pos = {0, 0}, Vector2Float size = {100, 50}) : _pos(pos), _size(size) {};
        private:
            /* private */
    
    };
} // namespace Vox::Front::Interfaces::Utils


#endif // __AELEMENT_HPP__