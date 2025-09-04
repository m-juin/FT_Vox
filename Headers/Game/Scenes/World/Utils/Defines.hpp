#ifndef __GAME_DEFINES_HPP__
#define __GAME_DEFINES_HPP__

#include "Front/Rendering/Utils/Buffers/DynamicBuffer.hpp"
#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"
#include "MathGraphicalLib/Vectors/Vector3.hpp"
#include "MathGraphicalLib/Vectors/Vector2.hpp"
#include "MathGraphicalLib/Matrix/Matrix4.hpp"

namespace Vox::Game::Utils::Defines
{
    using dbuffer = Vox::Front::Rendering::Utils::Buffers::DynamicBuffer;
    using sbuffer = Vox::Front::Rendering::Utils::Buffers::StaticBuffer;
    using Vertex = Vox::Front::Rendering::Utils::Vertex::VoxelVertex;
    using Vector3Float = MGL::Vectors::Vector3<float>;
    using Vector3Int = MGL::Vectors::Vector3<int>;
    using ChunckCoord = MGL::Vectors::Vector2<int>;
    using Matrix = MGL::Matrix::Matrix4;

    
    constexpr size_t RENDER_DISTANCE = 12;
    constexpr size_t SQUARE_RENDER_DISTANCE = RENDER_DISTANCE * RENDER_DISTANCE;
    constexpr int HALF_RENDER_DISTANCE = (RENDER_DISTANCE / 2);
    constexpr size_t CHUNCK_SIZE = 16;
    constexpr size_t WORLD_HEIGHT = 256;

    constexpr size_t CHUNCK_AMOUNT = (RENDER_DISTANCE * RENDER_DISTANCE) * (WORLD_HEIGHT / CHUNCK_SIZE);
} // namespace Vox::Game::Utils::Defines


#endif // __DEFINES_HPP__