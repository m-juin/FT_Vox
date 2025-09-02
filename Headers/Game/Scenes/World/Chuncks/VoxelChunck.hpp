#ifndef __VOXELCHUNCK_HPP__
#define __VOXELCHUNCK_HPP__

#include "MathGraphicalLib/Matrix/Matrix4.hpp"

#include "Game/Scenes/World/Utils/Defines.hpp"

#include "Game/Models/DynamicObject.hpp"

namespace Vox::Game::World::Chuncks
{
    using namespace Game::Utils::Defines;
    class VoxelChunck : public Models::DynamicObject
    {
        public:
            struct ChunckUniform
            {
                MGL::Matrix::Matrix4 model;
            };

            VoxelChunck() = delete;
            void AssignModel() override;
            VoxelChunck(size_t bufferIndex);
            void BuildVoxelObject();
            ~VoxelChunck();
    
            void Render();

        private:

            size_t _bufferIndex;
            sbuffer *B_Vertex;
            sbuffer *B_Index;

            /* private */
    
    };
} // namespace Vox::Game::Chuncks


#endif // __VOXELCHUNCK_HPP__