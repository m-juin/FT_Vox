#ifndef __VOXELMESH_HPP__
#define __VOXELMESH_HPP__

#include <vector>

#include "Engine/Rendering/Buffers/Buffer.hpp"
#include "Engine/Rendering/IDrawable.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

namespace Vox::Engine::Meshs
{
	using Vertex = Vox::Front::Rendering::Utils::Vertex::VoxelVertex;
	class VoxelMesh : public Rendering::IDrawable
	{

		private:
			template <typename T>
            struct MemoryData
			{
					size_t elemCount = 0;
					VkDeviceSize memorySize = 0;
					VkDeviceSize memoryOffset = 0;
					VkDeviceSize memoryReserved = 0;

					std::vector<T> datas;
			};

			MemoryData<Vertex> _vertexMemData;
			MemoryData<uint16_t> _indexMemData;

		public:
			VoxelMesh();
			~VoxelMesh();

			void SetMeshDatas(const std::vector<Vertex> &, const std::vector<uint16_t> &);

			bool Draw() override;

			const MemoryData<Vertex> GetVertexDatas()
			{
				return this->_vertexMemData;
			}
			const MemoryData<uint16_t> GetIndexDatas()
			{
				return this->_indexMemData;
			}

			void SetMemoryDatas(VkDeviceSize, VkDeviceSize, VkDeviceSize, VkDeviceSize);

			void WriteInBuffers(Engine::Rendering::Buffers::Buffer &, Engine::Rendering::Buffers::Buffer &);

			/* private */
	};
} // namespace Vox::Engine::Meshs

#endif // __VOXELMESH_HPP__