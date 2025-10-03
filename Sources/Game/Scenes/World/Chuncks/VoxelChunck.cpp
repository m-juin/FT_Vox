#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"
#include "Game/Scenes/World/Chuncks/GenerationUtils.hpp"

#include <array>

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

#include <bitset>

#include "Game/Scenes/World/Generation/BiomesPerlin.hpp"
#include "Game/Scenes/World/Generation/PerlinInterpretation.hpp"

#include "Game/Utils/Datas/Biomes.hpp"
#include "Game/Utils/Datas/BiomesData/SurfaceDecoration.hpp"

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

	void VoxelChunck::BuildVoxelObject(
		const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl,
		const std::vector<Game::Utils::Textures::TextureInfo> &textInfo, const Generation::Utils::ChunckCache &cache,
		const uint32_t &seed)
	{
		(void)textInfo;
		std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> clusterContent = this->BuildContent(cache.heightMap);

		LocalVector it(0);

		auto checkFace = [this, &clusterContent, &seed, &spl, &textInfo,
						  &cache](const LocalVector &it, int offsetX, int offsetY, int offsetZ, Faces face,
								  const std::string &blockType, const Vector3Float &color)
		{
			(void)seed;
			(void)spl;
			// LocalVector neighbor = it;
			MGL::Vectors::Vector3<int> neighbor = {it[0] + offsetX, it[1] + offsetY, it[2] + offsetZ};
			// neighbor[0] += offsetX;
			// neighbor[1] += offsetY;
			// neighbor[2] += offsetZ;

			// const std::string blockType = "Grass";

			if (neighbor[0] >= 0 && neighbor[0] < static_cast<int>(CHUNCK_SIZE) && neighbor[1] >= 0 && neighbor[1] < static_cast<int>(CHUNCK_SIZE) &&
				neighbor[2] >= 0 && neighbor[2] < static_cast<int>(CHUNCK_SIZE))
			{
				uint16_t neighborIndex = GetLocalIndex(LocalVector(neighbor[0], neighbor[1], neighbor[2]));
				if (!clusterContent[neighborIndex])
					this->AddFace(textInfo, face, it, blockType, color);
				return;
			}

			else
			{
				size_t neighborIndex = (neighbor[0] + Generation::Utils::GENERATION_BLEND_RADIUS) * Generation::Utils::CACHE_SIZE + (neighbor[2] + Generation::Utils::GENERATION_BLEND_RADIUS);
				int neighborWorldY = this->_position[1] + neighbor[1];
				uint8_t neighborColHeight = cache.heightMap[neighborIndex];

				if ( neighborWorldY > neighborColHeight)
					this->AddFace(textInfo, face, it, blockType, color);
			}
		};

		for (it[0] = 0; it[0] < CHUNCK_SIZE; it[0]++)
		{
			for (it[2] = 0; it[2] < CHUNCK_SIZE; it[2]++)
			{
				size_t cacheIndex =
					(it[0] + Generation::Utils::GENERATION_BLEND_RADIUS) * Generation::Utils::CACHE_SIZE +
					(it[2] + Generation::Utils::GENERATION_BLEND_RADIUS);
				Game::Generation::Datas::Biomes::Biomes biome = cache.biome[cacheIndex];
				uint8_t worldHeight = cache.heightMap[cacheIndex];
				for (it[1] = 0; it[1] < CHUNCK_SIZE; it[1]++)
				{
					uint16_t mapIndex = GetLocalIndex(it);
					if (!clusterContent[mapIndex])
						continue;

					std::string blockType;
					Vector3Float color = {1.0, 1.0, 1.0};
					blockType = Game::Generation::Datas::Biomes::GetBlockType(
						biome, (int)worldHeight - (this->_position[1] + (int)it[1]));
					if (blockType == "Grass")
					{
						Vector3Int biomeColor = Game::Generation::Datas::Biomes::biomesColors[biome];
						color = {static_cast<float>(biomeColor[0]) / 256.0f, static_cast<float>(biomeColor[1]) / 256.0f,
								 static_cast<float>(biomeColor[2]) / 256.0f};
						// std::cout << color << std::endl;
					}
					checkFace(it, 0, 1, 0, Faces::TOP, blockType, color);
					// checkFace(it, 0, -1, 0, Faces::BOT);
					checkFace(it, -1, 0, 0, Faces::LEFT, blockType, color);
					checkFace(it, 1, 0, 0, Faces::RIGHT, blockType, color);
					checkFace(it, 0, 0, 1, Faces::FRONT, blockType, color);
					checkFace(it, 0, 0, -1, Faces::BACK, blockType, color);
				}
			}
		}
	}

	std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> VoxelChunck::BuildContent(
		const uint8_t hMap[Generation::Utils::CACHE_SIZE * Generation::Utils::CACHE_SIZE])
	{
		std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> clusterContent;

		LocalVector it(0);
		for (it[0] = 0; it[0] < Utils::Defines::CHUNCK_SIZE; it[0]++)
		{
			for (it[2] = 0; it[2] < Utils::Defines::CHUNCK_SIZE; it[2]++)
			{
				uint8_t target =
					hMap[(it[0] + Generation::Utils::GENERATION_BLEND_RADIUS) * Generation::Utils::CACHE_SIZE +
						 (it[2] + Generation::Utils::GENERATION_BLEND_RADIUS)];
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
		using namespace Front::Rendering;
		auto frame = SyncObjects::GetInstance().GetCurrentFrame();
		if (this->indexCount == 0 || this->_isDirty[frame] == true)
			return;
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

	void VoxelChunck::AddFace(const std::vector<Game::Utils::Textures::TextureInfo> &textInfo, const Faces &face,
							  const LocalVector &facePos, const std::string &blockType, const Vector3Float &faceColor)
	{
		std::array<Vertex, 4> toAdd = defaultFacesPos.at(face);

		auto texture = Game::Generation::Utils::GetFaceTextureInfo(textInfo, blockType, face);

		for (auto &ref : toAdd)
		{
			for (uint8_t i = 0; i < 3; i++)
			{
				ref.vertPos[i] += facePos[i];
				ref.vertColor = faceColor;
			}
			ref.vertCoord[0] = texture.uOffset + ref.vertCoord[0] * texture.uSize;
			ref.vertCoord[1] = texture.vOffset + ref.vertCoord[1] * texture.vSize;
			if (blockType == "Grass")
				ref.isColorAffected = 1;
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