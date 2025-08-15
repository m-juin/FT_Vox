#ifndef __AELEMENT_HPP__
#define __AELEMENT_HPP__

#include <cstdint>

#include "MathGraphicalLib/Vectors/Vector2.hpp"

using Vector2 = MGL::Vectors::Vector2<size_t>;

namespace Vox::Front::Interfaces::Elements::Bases
{
    class AElement
    {
        public:
            virtual void Draw() = 0;
            virtual ~AElement() {};
            virtual void SetPos(const Vector2 newPos) = 0;
            virtual void SetSize(const Vector2 newSize) = 0;
        protected:
            Vector2 _pos;
            Vector2 _size;
            AElement(Vector2 pos = {0, 0}, Vector2 size = {100, 50}) : _pos(pos), _size(size) {};
        private:
            /* private */
    
    };
} // namespace Vox::Front::Interfaces::Utils


#endif // __AELEMENT_HPP__