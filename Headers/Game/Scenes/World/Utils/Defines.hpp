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

        
    constexpr int RENDER_DISTANCE = 11;
    constexpr int SQUARE_RENDER_DISTANCE = (RENDER_DISTANCE * RENDER_DISTANCE);
    constexpr int HALF_RENDER_DISTANCE = (RENDER_DISTANCE / 2);
    constexpr int HALF_RENDER_DISTANCE_SQUARE = HALF_RENDER_DISTANCE * HALF_RENDER_DISTANCE ;
    constexpr size_t CHUNCK_SIZE = 16;
    constexpr size_t WORLD_HEIGHT = 256;

    constexpr size_t CHUNCK_AMOUNT = ((RENDER_DISTANCE * 2) * (RENDER_DISTANCE * 2)) * (WORLD_HEIGHT / CHUNCK_SIZE);
    constexpr size_t CHUNCK_BUFFER_AMOUNT = ((RENDER_DISTANCE * 2) * (RENDER_DISTANCE * 2));
} // namespace Vox::Game::Utils::Defines


#endif // __GAME_DEFINES_HPP__