#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"
#include "Game/Scenes/World/Chuncks/GenerationUtils.hpp"

#include <array>

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/TransparentVoxelPipeline.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

#include <bitset>

#include "Game/Scenes/World/Generation/BiomesPerlin.hpp"
#include "Game/Scenes/World/Generation/PerlinInterpretation.hpp"
#include "Game/Scenes/World/Generation/RulesManager.hpp"

#include "Game/Datas/Biomes.hpp"

namespace Vox::Game::World::Chuncks
{
	VoxelChunck::VoxelChunck(const Vector3Int &defaultPos)
		: DynamicObject(Vector3Float(defaultPos[0] * static_cast<int>(Utils::Defines::CHUNCK_SIZE),
									 defaultPos[1] * static_cast<int>(Utils::Defines::CHUNCK_SIZE),
									 defaultPos[2] * static_cast<int>(Utils::Defines::CHUNCK_SIZE))),
		  _chunckPos(defaultPos)
	{
		this->B_IndexOpaque = nullptr;
		this->B_VertexOpaque = nullptr;

		this->B_VertexTransparent = nullptr;
		this->B_IndexTransparent = nullptr;

		indexCountOpaque = 0;
		indexCountTransparent = 0;
	}

	void VoxelChunck::AssignModel()
	{
		auto &wm = WorldManager::GetInstance();
		wm.UpdateBuffer(this->_bufferIndex, {this->GetModel()});
	}

	void VoxelChunck::BuildVoxelObject(
		const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl,
		const std::vector<Game::Datas::Textures::TextureInfo> &textInfo,
		const std::vector<Game::Datas::Textures::TextureInfo> &transparenttextInfo,
		const Generation::Utils::ChunckCache &cache, const uint32_t &seed)
	{
		(void)spl;
		(void)seed;
		(void)textInfo;
		(void)transparenttextInfo;
		std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> clusterContent = this->BuildContent(cache.heightMap);
		this->SetBlocksDatas(cache, clusterContent);
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

	void VoxelChunck::SetBlocksDatas(const Generation::Utils::ChunckCache &cache,
									 std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> &clusterContent)
	{
		LocalVector it(0);
		for (it[0] = 0; it[0] < CHUNCK_SIZE; it[0]++)
		{
			for (it[2] = 0; it[2] < CHUNCK_SIZE; it[2]++)
			{
				size_t cacheIndex =
					(it[0] + Generation::Utils::GENERATION_BLEND_RADIUS) * Generation::Utils::CACHE_SIZE +
					(it[2] + Generation::Utils::GENERATION_BLEND_RADIUS);
				Game::Datas::Biomes::Biomes biome = cache.biome[cacheIndex];
				uint8_t worldHeight = cache.heightMap[cacheIndex];
				for (it[1] = 0; it[1] < CHUNCK_SIZE; it[1]++)
				{
					uint16_t mapIndex = GetLocalIndex(it);
					size_t h = this->_position[1] + (int)it[1];
					if (!clusterContent[mapIndex])
					{
						if (h > Generation::Utils::WATER_LEVEL)
						{
							this->_blocksDatas[mapIndex].type = Game::Datas::Blocks::BlockType::Air;
							if (it[0] > 0)
								this->_blocksDatas[GetLocalIndex({static_cast<uint8_t>(it[0] - 1), it[1], it[2]})].UpdateFaceVisibility(
									true, Faces::RIGHT);
							if (it[0] < CHUNCK_SIZE - 1)
								this->_blocksDatas[GetLocalIndex({static_cast<uint8_t>(it[0] + 1), it[1], it[2]})].UpdateFaceVisibility(
									true, Faces::LEFT);
							if (it[1] > 0)
								this->_blocksDatas[GetLocalIndex({it[0], static_cast<uint8_t>(it[1] - 1), it[2]})].UpdateFaceVisibility(
									true, Faces::TOP);
							if (it[1] < CHUNCK_SIZE - 1)
								this->_blocksDatas[GetLocalIndex({it[0], static_cast<uint8_t>(it[1] + 1), it[2]})].UpdateFaceVisibility(
									true, Faces::BOT);
							if (it[2] > 0)
								this->_blocksDatas[GetLocalIndex({it[0], it[1], static_cast<uint8_t>(it[2] - 1)})].UpdateFaceVisibility(
									true, Faces::FRONT);
							if (it[2] < CHUNCK_SIZE - 1)
								this->_blocksDatas[GetLocalIndex({it[0], it[1], static_cast<uint8_t>(it[2] + 1)})].UpdateFaceVisibility(
									true, Faces::BACK);
						}
						else
						{
							this->_blocksDatas[mapIndex].type = Game::Datas::Blocks::BlockType::Water;
							this->_blocksDatas[mapIndex].UpdateFacesTransparency({true, true, true, true, true, true});
						}
					}
					else
					{
						this->_blocksDatas[mapIndex].type =
							Generation::Datas::Biomes::RulesManager::GetBlockType(biome, (int)worldHeight - (h));
					}
				}
			}
		}
	}

	void VoxelChunck::SetBlockDatas(const LocalVector &localPos, Vox::Game::Datas::Blocks::BlockType newType)
	{
		size_t index = this->GetLocalIndex(localPos);
		this->_blocksDatas[index].type = newType;
		if (newType == Game::Datas::Blocks::BlockType::Air)
		{
			this->_blocksDatas[index].type = Game::Datas::Blocks::BlockType::Air;
			this->_blocksDatas[index].UpdateFacesTransparency({true, true, true, true, true, true});
			// if (localPos[0] > 0)
			// 	this->_blocksDatas[GetLocalIndex({static_cast<uint8_t>(localPos[0] - 1), localPos[1], localPos[2]})].UpdateFaceVisibility(
			// 		true, Faces::RIGHT);
			// if (localPos[0] < CHUNCK_SIZE - 1)
			// 	this->_blocksDatas[GetLocalIndex({static_cast<uint8_t>(localPos[0] + 1), localPos[1], localPos[2]})].UpdateFaceVisibility(
			// 		true, Faces::LEFT);
			// if (localPos[1] > 0)
			// 	this->_blocksDatas[GetLocalIndex({localPos[0], static_cast<uint8_t>(localPos[1] - 1), localPos[2]})].UpdateFaceVisibility(
			// 		true, Faces::TOP);
			// if (localPos[1] < CHUNCK_SIZE - 1)
			// 	this->_blocksDatas[GetLocalIndex({localPos[0], static_cast<uint8_t>(localPos[1] + 1), localPos[2]})].UpdateFaceVisibility(
			// 		true, Faces::BOT);
			// if (localPos[2] > 0)
			// 	this->_blocksDatas[GetLocalIndex({localPos[0], localPos[1], static_cast<uint8_t>(localPos[2] - 1)})].UpdateFaceVisibility(
			// 		true, Faces::FRONT);
			// if (localPos[2] < CHUNCK_SIZE - 1)
			// 	this->_blocksDatas[GetLocalIndex({localPos[0], localPos[1], static_cast<uint8_t>(localPos[2] + 1)})].UpdateFaceVisibility(
			// 		true, Faces::BACK);
		}
		else
		{
			this->_blocksDatas[index].type = Game::Datas::Blocks::BlockType::Water;
			this->_blocksDatas[index].UpdateFacesTransparency({true, true, true, true, true, true});
		}
	}

	Vox::Game::Datas::Blocks::BlockType VoxelChunck::GetBlockDatas(const LocalVector &localPos)
	{
		size_t index = this->GetLocalIndex(localPos);
		return this->_blocksDatas[index].type;
	}

	// void VoxelChunck::BuildMesh(const std::vector<Game::Datas::Textures::TextureInfo> &textInfo,
	// 							const std::vector<Game::Datas::Textures::TextureInfo> &transparenttextInfo,
	// 							const Generation::Utils::ChunckCache &cache)
	// {
	// 	auto checkFace = [this, &textInfo, &cache](const LocalVector &it, int offsetX, int offsetY, int offsetZ,
	// 											   Faces face, const Game::Datas::Blocks::BlockType &blockType,
	// 											   const Vector3Float &color)
	// 	{
	// 		MGL::Vectors::Vector3<int> neighbor = {it[0] + offsetX, it[1] + offsetY, it[2] + offsetZ};

	// 		if (neighbor[0] >= 0 && neighbor[0] < static_cast<int>(CHUNCK_SIZE) && neighbor[1] >= 0 &&
	// 			neighbor[1] < static_cast<int>(CHUNCK_SIZE) && neighbor[2] >= 0 &&
	// 			neighbor[2] < static_cast<int>(CHUNCK_SIZE))
	// 		{
	// 			uint16_t neighborIndex = GetLocalIndex(LocalVector(neighbor[0], neighbor[1], neighbor[2]));
	// 			Vox::Game::Datas::Blocks::BlockType neightborType = this->_blocksDatas[neighborIndex].type;
	// 			if (neightborType == Vox::Game::Datas::Blocks::BlockType::Air ||
	// 				neightborType == Vox::Game::Datas::Blocks::BlockType::Water)
	// 				this->AddFace(textInfo, face, it, blockType, color);
	// 			return;
	// 		}

	// 		else
	// 		{
	// 			size_t neighborIndex =
	// 				(neighbor[0] + Generation::Utils::GENERATION_BLEND_RADIUS) * Generation::Utils::CACHE_SIZE +
	// 				(neighbor[2] + Generation::Utils::GENERATION_BLEND_RADIUS);
	// 			int neighborWorldY = this->_position[1] + neighbor[1];
	// 			uint8_t neighborColHeight = cache.heightMap[neighborIndex];

	// 			if (neighborWorldY > neighborColHeight)
	// 				this->AddFace(textInfo, face, it, blockType, color);
	// 		}
	// 	};

	// 	auto checkFaceWater = [this, &transparenttextInfo](const LocalVector &it, int offsetX, int offsetY, int offsetZ,
	// 													   Faces face, const Vector3Float &color)
	// 	{
	// 		MGL::Vectors::Vector3<int> neighbor = {it[0] + offsetX, it[1] + offsetY, it[2] + offsetZ};

	// 		if (this->_position[1] + neighbor[1] > static_cast<int>(Generation::Utils::WATER_LEVEL))
	// 		{
	// 			this->AddFace(transparenttextInfo, face, it, Game::Datas::Blocks::BlockType::Water, color, true, 1.1);
	// 		}
	// 	};

	// 	LocalVector it(0);
	// 	for (it[0] = 0; it[0] < CHUNCK_SIZE; it[0]++)
	// 	{
	// 		for (it[2] = 0; it[2] < CHUNCK_SIZE; it[2]++)
	// 		{
	// 			size_t cacheIndex =
	// 				(it[0] + Generation::Utils::GENERATION_BLEND_RADIUS) * Generation::Utils::CACHE_SIZE +
	// 				(it[2] + Generation::Utils::GENERATION_BLEND_RADIUS);
	// 			Game::Datas::Biomes::Biomes biome = cache.biome[cacheIndex];
	// 			for (it[1] = 0; it[1] < CHUNCK_SIZE; it[1]++)
	// 			{
	// 				uint16_t mapIndex = GetLocalIndex(it);
	// 				Game::Datas::Blocks::BlockType _type = this->_blocksDatas[mapIndex].type;
	// 				if (_type == Vox::Game::Datas::Blocks::BlockType::Air)
	// 					continue;
	// 				else if (_type == Vox::Game::Datas::Blocks::BlockType::Water)
	// 				{
	// 					Vector3Float color = {1.0, 1.0, 1.0};
	// 					Vector3Int biomeColor = Game::Datas::Biomes::biomesColors[biome];
	// 					color = {static_cast<float>(biomeColor[0]) / 256.0f, static_cast<float>(biomeColor[1]) / 256.0f,
	// 							 static_cast<float>(biomeColor[2]) / 256.0f};
	// 					checkFaceWater(it, 0, 1, 0, Faces::TOP, color);
	// 					continue;
	// 				}
	// 				Vector3Float color = {1.0, 1.0, 1.0};
	// 				if (_type == Game::Datas::Blocks::BlockType::Grass)
	// 				{
	// 					Vector3Int biomeColor = Game::Datas::Biomes::biomesColors[biome];
	// 					color = {static_cast<float>(biomeColor[0]) / 256.0f, static_cast<float>(biomeColor[1]) / 256.0f,
	// 							 static_cast<float>(biomeColor[2]) / 256.0f};
	// 				}
	// 				checkFace(it, 0, 1, 0, Faces::TOP, _type, color);
	// 				checkFace(it, -1, 0, 0, Faces::LEFT, _type, color);
	// 				checkFace(it, 1, 0, 0, Faces::RIGHT, _type, color);
	// 				checkFace(it, 0, 0, 1, Faces::FRONT, _type, color);
	// 				checkFace(it, 0, 0, -1, Faces::BACK, _type, color);
	// 			}
	// 		}
	// 	}
	// }

	void VoxelChunck::BuildMesh(const std::vector<Game::Datas::Textures::TextureInfo> &textInfo,
								const std::vector<Game::Datas::Textures::TextureInfo> &transparenttextInfo,
								const Generation::Utils::ChunckCache &cache)
	{
		(void)cache;
		LocalVector it(0);
		Vector3Float color(1.0, 1.0, 1.0);
		for (it[0] = 0; it[0] < CHUNCK_SIZE; it[0]++)
		{
			for (it[2] = 0; it[2] < CHUNCK_SIZE; it[2]++)
			{
				for (it[1] = 0; it[1] < CHUNCK_SIZE; it[1]++)
				{
					uint16_t mapIndex = GetLocalIndex(it);
					auto data = this->_blocksDatas[mapIndex];
					auto faces = data.GetFacesData();
					for (auto face : faces)
					{
						if (face.isVisible == false)
							continue;
						if (face.isTransparent)
							this->AddFace((face.isTransparent ? transparenttextInfo : textInfo), face.faceDirection, it, data.type, color, face.isTransparent);
					}
				}
			}
		}
	}

	void VoxelChunck::BuildBufferObject(const uint16_t &buffer)
	{
		this->_bufferIndex = buffer;
		if (this->B_IndexOpaque != nullptr)
		{
			delete this->B_VertexOpaque;
			this->B_VertexOpaque = nullptr;
			delete this->B_IndexOpaque;
			this->B_IndexOpaque = nullptr;
		}
		if (this->vertexOpaque.size() == 0 && indexTransparent.size() == 0)
		{
			return;
		}
		if (vertexOpaque.size() != 0)
		{
			this->B_IndexOpaque =
				new sbuffer(2, indexOpaque.size() * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
			this->B_VertexOpaque =
				new sbuffer(2, vertexOpaque.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			this->indexCountOpaque = indexOpaque.size();
			this->B_IndexOpaque->Create(indexOpaque.data());
			this->B_VertexOpaque->Create(vertexOpaque.data());
			this->indexOpaque.clear();
			this->vertexOpaque.clear();
		}

		if (indexTransparent.size() != 0)
		{
			this->B_IndexTransparent =
				new sbuffer(2, indexTransparent.size() * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
			this->B_VertexTransparent =
				new sbuffer(2, vertexTransparent.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
			this->indexCountTransparent = indexTransparent.size();
			this->B_IndexTransparent->Create(indexTransparent.data());
			this->B_VertexTransparent->Create(vertexTransparent.data());
			this->indexTransparent.clear();
			this->vertexTransparent.clear();
		}
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

	void VoxelChunck::Render(uint8_t toRender)
	{
		using namespace Front::Rendering;
		auto frame = SyncObjects::GetInstance().GetCurrentFrame();
		if ((this->indexCountOpaque == 0 && this->indexCountTransparent == 0) || this->_isDirty[frame] == true)
			return;
		uint32_t dynamicOffset = this->_bufferIndex * Utils::Vulkan::GetAlignedChunckSize();

		auto buffer = CommandsPool::GetInstance().GetBuffer(frame);
		if (toRender == 0 && this->indexCountOpaque != 0)
		{
			auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::VoxelPipeline>("Voxel");
			if (pipeline == nullptr)
				return;
			VkDeviceSize offset = {0};
			vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1,
									&pipeline->GetSet(frame), 1, &dynamicOffset);
			vkCmdBindVertexBuffers(buffer, 0, 1, &this->B_VertexOpaque->GetBuffer(frame), &offset);
			vkCmdBindIndexBuffer(buffer, this->B_IndexOpaque->GetBuffer(frame), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(buffer, indexCountOpaque, 1, 0, 0, 0);
		}
		else if (toRender == 1 && this->indexCountTransparent != 0)
		{
			auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::TransparentVoxelPipeline>(
				"Voxel_Transparent");
			if (pipeline == nullptr)
				return;
			VkDeviceSize offset = {0};
			vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1,
									&pipeline->GetSet(frame), 1, &dynamicOffset);
			vkCmdBindVertexBuffers(buffer, 0, 1, &this->B_VertexTransparent->GetBuffer(frame), &offset);
			vkCmdBindIndexBuffer(buffer, this->B_IndexTransparent->GetBuffer(frame), 0, VK_INDEX_TYPE_UINT16);
			vkCmdDrawIndexed(buffer, indexCountTransparent, 1, 0, 0, 0);
		}
	}

	uint16_t VoxelChunck::GetBuffer() const
	{
		return this->_bufferIndex;
	}

	void VoxelChunck::AddFace(const std::vector<Game::Datas::Textures::TextureInfo> &textInfo, const Faces &face,
							  const LocalVector &facePos, const Game::Datas::Blocks::BlockType &blockType,
							  const Vector3Float &faceColor, bool target, float faceOffsef)
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
			ref.vertPos[1] += (1 - faceOffsef);
			ref.vertCoord[0] = texture.uOffset + ref.vertCoord[0] * texture.uSize;
			ref.vertCoord[1] = texture.vOffset + ref.vertCoord[1] * texture.vSize;
			if (blockType == Game::Datas::Blocks::BlockType::Grass)
				ref.isColorAffected = 1;
		}

		if (target == false)
		{
			auto beg = vertexOpaque.end();
			vertexOpaque.insert(beg, toAdd.begin(), toAdd.end());
			uint32_t size = indexOpaque.empty() ? 0 : indexOpaque[indexOpaque.size() - 1] + 1;
			indexOpaque.push_back(size);
			indexOpaque.push_back(size + 1);
			indexOpaque.push_back(size + 2);
			indexOpaque.push_back(size);
			indexOpaque.push_back(size + 2);
			indexOpaque.push_back(size + 3);
		}
		else
		{
			auto beg = vertexTransparent.end();
			vertexTransparent.insert(beg, toAdd.begin(), toAdd.end());
			uint32_t size = indexTransparent.empty() ? 0 : indexTransparent[indexTransparent.size() - 1] + 1;
			indexTransparent.push_back(size);
			indexTransparent.push_back(size + 1);
			indexTransparent.push_back(size + 2);
			indexTransparent.push_back(size);
			indexTransparent.push_back(size + 2);
			indexTransparent.push_back(size + 3);
		}
	}

	VoxelChunck::~VoxelChunck()
	{
		if (this->B_IndexOpaque)
			delete this->B_IndexOpaque;
		if (this->B_VertexOpaque)
			delete this->B_VertexOpaque;
		if (this->B_VertexTransparent)
			delete this->B_VertexTransparent;
		if (this->B_IndexTransparent)
			delete this->B_IndexTransparent;
	}
} // namespace Vox::Game::World::Chuncks