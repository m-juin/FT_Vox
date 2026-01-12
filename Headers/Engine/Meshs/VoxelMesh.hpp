#ifndef __VOXELMESH_HPP__
#define __VOXELMESH_HPP__

#include <vector>

#include "Engine/Rendering/IDrawable.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

namespace Vox::Engine::Meshs
{
    using Vertex = Vox::Front::Rendering::Utils::Vertex::VoxelVertex;
    class VoxelMesh : public Rendering::IDrawable
    {
        public:
            VoxelMesh();
            ~VoxelMesh();

            void SetMeshDatas(const std::vector<Vertex> &, const std::vector<uint16_t> &);

            bool Draw() override;

        private:
            struct MemoryData {
                size_t elemCount = 0;
                VkDeviceSize memorySize = 0;
                VkDeviceSize memoryOffset = 0;
            };

            MemoryData _vertexMemData;
            MemoryData _indexMemData;

            /* private */
    
    };
} // namespace Vox::Engine


#endif // __VOXELMESH_HPP__