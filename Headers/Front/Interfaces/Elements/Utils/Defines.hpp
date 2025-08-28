#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include "MathGraphicalLib/Vectors/Vector2.hpp"
#include "MathGraphicalLib/Vectors/Vector4.hpp"
#include "Front/Rendering/Utils/Vertex/StaticGUIVertex.hpp"
#include "Front/Interfaces/Utils/Maths.hpp"

#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"
#include "Front/Rendering/Utils/Buffers/DynamicBuffer.hpp"

namespace Vox::Front::Interfaces::Elements::Utils::Defines
{
    using Vector2 = MGL::Vectors::Vector2<float>;
    using Vertex = Front::Rendering::Utils::Vertex::StaticGUIVertex;

    using Color = MGL::Vectors::Vector4<float>;

    using dbuffer = Vox::Front::Rendering::Utils::Buffers::DynamicBuffer;
    using sbuffer = Vox::Front::Rendering::Utils::Buffers::StaticBuffer;

    using namespace Vox::Front::Interfaces::Utils::Maths;
} // namespace Vox::Front::interfaces::Elements::Utils


#endif // __DEFINES_HPP__