#include "Engine/Meshs/VoxelMesh.hpp"

namespace Vox::Engine::Meshs
{
	VoxelMesh::VoxelMesh() {}

	VoxelMesh::~VoxelMesh() {}

	void VoxelMesh::SetMeshDatas(const std::vector<Vertex> &vert, const std::vector<uint16_t> &idx)
	{
		this->_indexMemData.elemCount = idx.size();
		this->_indexMemData.memorySize = idx.size() * sizeof(uint16_t);
        this->_indexMemData.memoryOffset = 0;
        this->_indexMemData.memoryReserved = 0;

		this->_vertexMemData.elemCount = vert.size();
		this->_vertexMemData.memorySize = vert.size() * sizeof(Vertex);
        this->_vertexMemData.memoryOffset = 0;
        this->_vertexMemData.memoryReserved = 0;

	}

	bool VoxelMesh::Draw()
	{
		if (this->_indexMemData.elemCount == 0)
			return false;
		return true;
	}
	
	void VoxelMesh::SetMemoryDatas(VkDeviceSize vMemoryOffset, VkDeviceSize vMemoryReserved, VkDeviceSize iMemoryOffset, VkDeviceSize iMemoryReserved)
	{
		this->_vertexMemData.memoryOffset = vMemoryOffset;
		this->_vertexMemData.memoryReserved = vMemoryReserved;

		this->_indexMemData.memoryOffset = iMemoryOffset;
		this->_indexMemData.memoryReserved = iMemoryReserved;
	}
} // namespace Vox::Engine::Meshs