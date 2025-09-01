#ifndef __VOXELCHUNCK_HPP__
#define __VOXELCHUNCK_HPP__

#include "MathGraphicalLib/Matrix/Matrix4.hpp"

#include "Game/Scenes/World/Utils/Defines.hpp"

namespace Vox::Game::World::Chuncks
{
    using namespace Game::Utils::Defines;
    class VoxelChunck
    {
        public:
            struct ChunckUniform
            {
                MGL::Matrix::Matrix4 model;
            };
            
            VoxelChunck() = delete;
            VoxelChunck(size_t bufferIndex);
            void BuildVoxelObject();
            void BuildMatrix(Vector3 pos);
            ~VoxelChunck();
    
        private:
            
            size_t _bufferIndex;
            sbuffer *B_Vertex;
            sbuffer *B_Index;
            /* private */
    
    };
} // namespace Vox::Game::Chuncks


#endif // __VOXELCHUNCK_HPP__