#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"

#include <array>

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

#include <bitset>

#include "Game/Scenes/World/Generation/Perlin.hpp"

namespace Vox::Game::World::Chuncks
{
	VoxelChunck::VoxelChunck(const Vector3Int &defaultPos)
		: DynamicObject(Vector3Float(defaultPos[0] * static_cast<int>(Utils::Defines::CHUNCK_SIZE),
									 defaultPos[1] * static_cast<int>(Utils::Defines::CHUNCK_SIZE),
									 defaultPos[2] * static_cast<int>(Utils::Defines::CHUNCK_SIZE))),
		  _chunckPos(defaultPos)
	{
		this->B_Index = nullptr;
		this->B_Vertex = nullptr;

		indexCount = 0;
	}

	void VoxelChunck::AssignModel()
	{
		auto &wm = WorldManager::GetInstance();
		wm.UpdateBuffer(this->_bufferIndex, {this->GetModel()});
	}

	void VoxelChunck::BuildVoxelObject(const std::unordered_map<std::string, const Spline::Spline> &spl,
									   const uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE], const uint32_t &seed)
	{
		(void)spl;
		std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> clusterContent = this->BuildContent(hMap);

		LocalVector it(0);

		auto checkFace = [this, &clusterContent, &seed, &spl, &hMap](const LocalVector &it, int offsetX, int offsetY,
																	 int offsetZ, Faces face)
		{
			LocalVector neighbor = it;
			neighbor[0] += offsetX;
			neighbor[1] += offsetY;
			neighbor[2] += offsetZ;

			// voisin à l'intérieur du chunk => on regarde le bitset local
			if (neighbor[0] >= 0 && neighbor[0] < CHUNCK_SIZE && neighbor[1] >= 0 && neighbor[1] < CHUNCK_SIZE &&
				neighbor[2] >= 0 && neighbor[2] < CHUNCK_SIZE)
			{
				uint16_t neighborIndex = GetLocalIndex(neighbor);
				if (!clusterContent[neighborIndex]) // voisin vide
					this->AddFace(face, it);
				return;
			}

			// --- en dehors du chunk ---
			// Cas vertical (même XZ) : utilise le heightmap local pour rester cohérent
			if (offsetY != 0 && offsetX == 0 && offsetZ == 0)
			{
				// coords monde du voxel voisin
				int yWorld = static_cast<int>(this->_position[1]) + neighbor[1];
				int lx = it[0];
				int lz = it[2];
				uint8_t h = hMap[lx * CHUNCK_SIZE + lz];

				// même règle que BuildContent : plein si yWorld <= h
				bool neighborFilled = (yWorld <= static_cast<int>(h));
				if (!neighborFilled)
					this->AddFace(face, it);
				return;
			}
			if (!Generation::Perlins::IsBlockAt({static_cast<int>(this->_position[0]) + it[0] + offsetX,
												 static_cast<int>(this->_position[1]) + it[1] + offsetY,
												 static_cast<int>(this->_position[2]) + it[2] + offsetZ},
												seed, spl))
			{
				this->AddFace(face, it);
			}
		};

		// Parcours du chunk
		for (it[0] = 0; it[0] < CHUNCK_SIZE; it[0]++)
		{
			for (it[2] = 0; it[2] < CHUNCK_SIZE; it[2]++)
			{
				for (it[1] = 0; it[1] < CHUNCK_SIZE; it[1]++)
				{
					uint16_t mapIndex = GetLocalIndex(it);
					if (!clusterContent[mapIndex])
						continue;

					// au minimum TOP (les autres sont commentées chez toi pour debug)
					checkFace(it, 0, 1, 0, Faces::TOP);
					// une fois ok, réactive les autres directions :
					// checkFace(it, 0, -1, 0, Faces::BOT);
					checkFace(it, -1, 0, 0, Faces::LEFT);
					checkFace(it, 1, 0, 0, Faces::RIGHT);
					checkFace(it, 0, 0, 1, Faces::FRONT);
					checkFace(it, 0, 0, -1, Faces::BACK);
				}
			}
		}
	}

	std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> VoxelChunck::BuildContent(
		const uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE])
	{
		std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> clusterContent;

		LocalVector it(0);
		for (it[0] = 0; it[0] < Utils::Defines::CHUNCK_SIZE; it[0]++)
		{
			for (it[2] = 0; it[2] < Utils::Defines::CHUNCK_SIZE; it[2]++)
			{
				uint8_t target = hMap[it[0] * CHUNCK_SIZE + it[2]];
				for (it[1] = 0; it[1] < CHUNCK_SIZE; it[1]++)
				{
					if (it[1] + this->_position[1] <= target)
						clusterContent.set(GetLocalIndex(it));
				}
			}
		}

		return clusterContent;
	}

	void VoxelChunck::BuildBufferObject(const uint16_t &buffer)
	{
		this->_bufferIndex = buffer;
		if (this->B_Index != nullptr)
		{
			delete this->B_Vertex;
			this->B_Vertex = nullptr;
			delete this->B_Index;
			this->B_Index = nullptr;
		}
		if (this->vertex.size() == 0)
		{
			return;
		}
		this->B_Index = new sbuffer(2, index.size() * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		this->B_Vertex = new sbuffer(2, vertex.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		this->indexCount = index.size();
		this->B_Index->Create(index.data());
		this->B_Vertex->Create(vertex.data());
		this->index.clear();
		this->vertex.clear();
	}

	size_t VoxelChunck::GetLocalIndex(const LocalVector &vec)
	{
		return vec[0] + (vec[1] * CHUNCK_SIZE) + (vec[2] * (CHUNCK_SIZE * CHUNCK_SIZE));
	}

	VoxelChunck::LocalVector VoxelChunck::GetLocalVector(const size_t &index)
	{
		return LocalVector(
			index % Utils::Defines::CHUNCK_SIZE, (index / Utils::Defines::CHUNCK_SIZE) % Utils::Defines::CHUNCK_SIZE,
			((index / Utils::Defines::CHUNCK_SIZE) / Utils::Defines::CHUNCK_SIZE) % Utils::Defines::CHUNCK_SIZE);
	}

	void VoxelChunck::Render()
	{
		if (this->indexCount == 0)
			return;
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

	uint16_t VoxelChunck::GetBuffer() const
	{
		return this->_bufferIndex;
	}

	void VoxelChunck::AddFace(const Faces &face, const LocalVector &facePos)
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