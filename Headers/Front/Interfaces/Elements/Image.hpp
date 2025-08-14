#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include "Bases/AElement.hpp"

#include "Front/Rendering/Utils/Vertex/StaticGUIVertex.hpp"

using vert = Vox::Front::Rendering::Utils::Vertex::StaticGUIVertex;

namespace Vox::Front::Interfaces::Elements
{
    class Image : public Bases::AElement
    {
        private:
            vert vertex[6];
            /* data */
        public:
            Image(Vector2 pos = {0, 0}, Vector2 size = {100, 50});
            ~Image();
    };    
}

#endif // __IMAGE_HPP__