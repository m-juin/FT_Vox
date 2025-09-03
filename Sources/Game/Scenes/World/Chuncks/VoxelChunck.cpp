#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"

#include <array>

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

namespace Vox::Game::World::Chuncks
{
	VoxelChunck::VoxelChunck(size_t bufferIndex, const Vector3& defaultPos)
		: DynamicObject(defaultPos), _bufferIndex(bufferIndex)
	{
		this->B_Index = nullptr;
		this->B_Vertex = nullptr;

		indexCount = 0;
		this->_currentState = Generation::E_GenerationState::WaitingThread;
		BuildVoxelObject();
	}

	void VoxelChunck::AssignModel()
	{
		auto &wm = WorldManager::GetInstance();
		wm.UpdateBuffer(this->_bufferIndex, {this->GetModel()});
	}

	void VoxelChunck::BuildVoxelObject()
	{
		_currentState = Generation::E_GenerationState::Mesh;
		std::vector<Vertex> vertex;
		std::vector<uint16_t> index;

		for (size_t x = 0; x < Utils::Defines::CHUNCK_SIZE; x++)
		{
			for (size_t z = 0; z < Utils::Defines::CHUNCK_SIZE; z++)
			{
				for (size_t y = 0; y < Utils::Defines::CHUNCK_SIZE; y++)
				{
					if (y == Utils::Defines::CHUNCK_SIZE - 1)
					{
						this->AddFace(Faces::TOP, LocalVector(x, y, z), vertex, index);
					}
				}
			}
		}
		if (this->B_Index != nullptr)
		{
			delete this->B_Vertex;
			delete this->B_Index;
		}
		this->B_Index = new sbuffer(2, index.size() * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		this->B_Vertex = new sbuffer(2, vertex.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		this->indexCount = index.size();
		this->B_Index->Create(index.data());

		this->B_Vertex->Create(vertex.data());
		this->_currentState = Generation::E_GenerationState::End;
	}

	size_t VoxelChunck::GetLocalIndex(const LocalVector &vec)
	{
		return vec[0] + (vec[1] * CHUNCK_SIZE) +
			   (vec[2] * (CHUNCK_SIZE * CHUNCK_SIZE));
	}

	VoxelChunck::LocalVector VoxelChunck::GetLocalVector(const size_t &index)
	{
		return LocalVector(
			index % Utils::Defines::CHUNCK_SIZE, (index / Utils::Defines::CHUNCK_SIZE) % Utils::Defines::CHUNCK_SIZE,
			((index / Utils::Defines::CHUNCK_SIZE) / Utils::Defines::CHUNCK_SIZE) % Utils::Defines::CHUNCK_SIZE);
	}

	void VoxelChunck::Render()
	{
		if (this->_currentState != Generation::E_GenerationState::End) return ;
		using namespace Front::Rendering;
		auto frame = SyncObjects::GetInstance().GetCurrentFrame();
		uint32_t dynamicOffset = this->_bufferIndex * Utils::Vulkan::GetAlignedChunckSize();

		auto buffer = CommandsPool::GetInstance().GetBuffer(frame);
		auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::VoxelPipeline>("Voxel");
		if (pipeline == nullptr)
			return;
		VkDeviceSize offset = {0};
		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1,
								&pipeline->GetSet(frame), 1, &dynamicOffset);
		vkCmdBindVertexBuffers(buffer, 0, 1, &this->B_Vertex->GetBuffer(frame), &offset);
		vkCmdBindIndexBuffer(buffer, this->B_Index->GetBuffer(frame), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(buffer, indexCount, 1, 0, 0, 0);
	}

	uint16_t VoxelChunck::GetBuffer() const {return this->_bufferIndex;}

	void VoxelChunck::AddFace(const Faces &face, const LocalVector &facePos, std::vector<Vertex> &vertex,
	                          std::vector<uint16_t> &index)
	{
		std::array<Vertex, 4> toAdd = defaultFacesPos.at(face);
		for (auto &ref : toAdd)
		{
			for (uint8_t i = 0; i < 3; i++)
				ref.vertPos[i] += facePos[i];
		}
		auto beg = vertex.end();

		vertex.insert(beg, toAdd.begin(), toAdd.end());
		uint32_t size = index.empty() ? 0 : index[index.size() - 1] + 1;
		index.push_back(size);
		index.push_back(size + 1);
		index.push_back(size + 2);
		index.push_back(size);
		index.push_back(size + 2);
		index.push_back(size + 3);
	}

	VoxelChunck::~VoxelChunck()
	{
		if (this->B_Index)
			delete this->B_Index;
		if (this->B_Vertex)
			delete this->B_Vertex;
	}
} // namespace Vox::Game::World::Chuncks