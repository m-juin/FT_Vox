#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"

#include <array>

#include "Front/Rendering/SyncObjects.hpp"
#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

namespace Vox::Game::World::Chuncks
{
	VoxelChunck::VoxelChunck(size_t bufferIndex) : DynamicObject(), _bufferIndex(bufferIndex)
	{
		this->B_Index = nullptr;
		this->B_Vertex = nullptr;

		BuildVoxelObject();
	}

	void VoxelChunck::AssignModel()
	{
		auto &wm = WorldManager::GetInstance();
		wm.UpdateBuffer(this->_bufferIndex, {this->GetModel()});
	}

	void VoxelChunck::BuildVoxelObject()
	{
		std::array<Vertex, 8> vertex = {
			Vertex({0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}), Vertex({0.5, -0.5, 0.5}, {1.0, 1.0, 1.0}),
			Vertex({-0.5, -0.5, 0.5}, {1.0, 1.0, 1.0}), Vertex({-0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}),
			Vertex({0.5, 0.5, -0.5}, {1.0, 1.0, 1.0}),	Vertex({0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}),
			Vertex({-0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}),	Vertex({-0.5, 0.5, -0.5}, {1.0, 1.0, 1.0}),
		};
		std::array<uint16_t, 36> index = {0, 1, 2, 2, 3, 0,
										  // haut
										  4, 5, 6, 6, 7, 4,
										  // devant
										  1, 5, 6, 6, 2, 1,
										  // derrière
										  0, 4, 7, 7, 3, 0,
										  // droite
										  0, 1, 5, 5, 4, 0,
										  // gauche
										  3, 2, 6, 6, 7, 3};

		if (this->B_Index != nullptr)
		{
			delete this->B_Vertex;
			delete this->B_Index;
		}
		this->B_Index = new sbuffer(2, sizeof(index), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		this->B_Vertex = new sbuffer(2, sizeof(vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		this->B_Index->Create(index.data());

		this->B_Vertex->Create(vertex.data());
	}

	void VoxelChunck::Render()
	{
		using namespace Front::Rendering;
		auto frame = SyncObjects::GetInstance().GetCurrentFrame();
		uint32_t dynamicOffset = this->_bufferIndex * Utils::Vulkan::GetAlignedChunckSize();

		auto buffer = CommandsPool::GetInstance().GetBuffer(frame);
		auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::VoxelPipeline>("Voxel");
		if (pipeline == nullptr)
			return ;
		VkDeviceSize offset = {0};
		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1,
								&pipeline->GetSet(frame), 1, &dynamicOffset);
		vkCmdBindVertexBuffers(buffer, 0, 1, &this->B_Vertex->GetBuffer(frame), &offset);
		vkCmdBindIndexBuffer(buffer, this->B_Index->GetBuffer(frame), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(buffer, 36, 1, 0, 0, 0);
	}

	VoxelChunck::~VoxelChunck()
	{
		if (this->B_Index)
			delete this->B_Index;
		if (this->B_Vertex)
			delete this->B_Vertex;
	}
} // namespace Vox::Game::World::Chuncks