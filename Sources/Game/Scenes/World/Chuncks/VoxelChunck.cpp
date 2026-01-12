#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"
#include "Game/Scenes/World/Chuncks/GenerationUtils.hpp"

#include <array>

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/TransparentVoxelPipeline.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

// #include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

#include <bitset>

#include "Game/Scenes/World/Generation/BiomesPerlin.hpp"
#include "Game/Scenes/World/Generation/PerlinInterpretation.hpp"
#include "Game/Scenes/World/Generation/RulesManager.hpp"

#include "Game/Datas/Biomes.hpp"

#include "Front/Scenes/TexturesManager.hpp"
#include "Utils/TracyUtils.hpp"

#include "Game/Scenes/World/Generation/BufferMemoryManager.hpp"
#include "Game/Scenes/World/Generation/BuffersCleanupManager.hpp"

namespace Vox::Game::World::Chuncks
{
	bool VoxelChunck::Draw() {
		return false;
	}

	VoxelChunck::VoxelChunck(const Vector3Int &defaultPos)
		: DynamicObject(Vector3Float(defaultPos[0] * static_cast<int>(Utils::Defines::CHUNCK_SIZE),
									 defaultPos[1] * static_cast<int>(Utils::Defines::CHUNCK_SIZE),
									 defaultPos[2] * static_cast<int>(Utils::Defines::CHUNCK_SIZE))),
		  Front::Rendering::Frustum::Colliders::BoxCollider(
			  Vector3Float(defaultPos[0] * static_cast<int>(Utils::Defines::CHUNCK_SIZE),
						   defaultPos[1] * static_cast<int>(Utils::Defines::CHUNCK_SIZE),
						   defaultPos[2] * static_cast<int>(Utils::Defines::CHUNCK_SIZE)),
			  {CHUNCK_SIZE, CHUNCK_SIZE, CHUNCK_SIZE}),
		  _chunckPos(defaultPos)
	{
		this->_bufferIndex = CHUNCK_AMOUNT;

		// TODO Add this only after the end of the generation.
		// this->onUpdate.AddCallBack(
		// [this](void)
		// {
		// this->UpdateVisibility();
		// auto nFrame = Front::Rendering::SyncObjects::GetInstance().GetNextFrame();
		// UpdateBufferObject();
		// });
	}

	bool VoxelChunck::AssignModel()
	{
		if (this->_bufferIndex >= Utils::Defines::CHUNCK_AMOUNT)
			return false;
		auto &wm = WorldManager::GetInstance();
		wm.UpdateBuffer(this->_bufferIndex, {this->GetModel()});
		return true;
	}

	std::unique_ptr<std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE>> VoxelChunck::GEN_ContentBitset(
		const uint8_t hMap[Generation::Utils::CACHE_SIZE * Generation::Utils::CACHE_SIZE])
	{
		std::unique_ptr<std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE>> clusterContent =
			std::make_unique<std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE>>();

		Vector3Uint8 it(0);
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
						clusterContent->set(GetLocalIndex(it));
				}
			}
		}
		return clusterContent;
	}

	void VoxelChunck::SetBlockDatas(const Vector3Uint8 &localPos, Vox::Game::Datas::Blocks::BlockType newType)
	{
		(void)localPos;
		(void)newType;
		// size_t index = this->GetLocalIndex(localPos);
		// this->_blocksDatas[index].type = newType;
		// bool isTransparent = newType == Game::Datas::Blocks::BlockType::Air ||
		// 					 newType == Game::Datas::Blocks::BlockType::Water ||
		// 					 newType == Game::Datas::Blocks::BlockType::Oak_Leaves;
		// this->_blocksDatas[index].UpdateFacesTransparency(
		// 	{isTransparent, isTransparent, isTransparent, isTransparent, isTransparent, isTransparent});

		// const auto &tManagers = Vox::Front::Scenes::TexturesManager::GetInstance();
		// for (auto &face : this->_blocksDatas[index].GetFacesData())
		// {
		// 	this->AddFace((face.isTransparent ? tManagers.operator[]("A_Blocks_Transparent").GetTextureInfo()
		// 									  : tManagers.operator[]("A_Blocks").GetTextureInfo()),
		// 				  face.faceDirection, localPos, newType, {1.f, 1.f, 1.f}, isTransparent);
		// }
	}

	void VoxelChunck::GEN_FacesCulling(const Generation::Utils::ChunckCache &cache)
	{
		using Vox::Game::Datas::Blocks::BlockData;
		using Vox::Game::Datas::Blocks::BlockType;

		auto GetAdjacentBlockData = [this, &cache](const Vector3Uint8 &it, Faces face)
		{
			Vector3Int neighbor(it[0], it[1], it[2]);
			if (face == Faces::LEFT)
				neighbor[0] += -1;
			else if (face == Faces::RIGHT)
				neighbor[0] += 1;
			else if (face == Faces::BOT)
				neighbor[1] += -1;
			else if (face == Faces::TOP)
				neighbor[1] += 1;
			else if (face == Faces::BACK)
				neighbor[2] += -1;
			else
				neighbor[2] += 1;
			if ((it[0] != 0 || face != Faces::LEFT) && (it[0] != CHUNCK_SIZE - 1 || face != Faces::RIGHT) &&
				(it[1] != 0 || face != Faces::BOT) && (it[1] != CHUNCK_SIZE - 1 || face != Faces::TOP) &&
				(it[2] != 0 || face != Faces::BACK) && (it[2] != CHUNCK_SIZE - 1 || face != Faces::FRONT))
			{
				auto index = this->GetLocalIndex({static_cast<uint8_t>(neighbor[0]), static_cast<uint8_t>(neighbor[1]),
												  static_cast<uint8_t>(neighbor[2])});
				return this->_blocksDatas[index];
			}
			else
			{
				BlockData data;
				size_t neighborIndex =
					(neighbor[0] + Generation::Utils::GENERATION_BLEND_RADIUS) * Generation::Utils::CACHE_SIZE +
					(neighbor[2] + Generation::Utils::GENERATION_BLEND_RADIUS);
				int neighborWorldY = this->_position[1] + neighbor[1];
				uint8_t neighborColHeight = cache.heightMap[neighborIndex];
				if (neighborWorldY > neighborColHeight)
				{
					data.type = neighborWorldY > static_cast<int>(Generation::Utils::WATER_LEVEL) ? BlockType::Air
																								  : BlockType::Water;
					data.UpdateFacesTransparency({true, true, true, true, true, true});
				}
				else
				{
					data.type = BlockType::Dirt;
					data.UpdateFacesTransparency({false, false, false, false, false, false});
				}
				return data;
			}
		};
		Vector3Uint8 it;
		for (it[0] = 0; it[0] < CHUNCK_SIZE; it[0]++)
		{
			for (it[2] = 0; it[2] < CHUNCK_SIZE; it[2]++)
			{
				for (it[1] = 0; it[1] < CHUNCK_SIZE; it[1]++)
				{
					uint16_t mapIndex = GetLocalIndex(it);
					auto &bData = this->_blocksDatas[mapIndex];
					if (bData.type == BlockType::Air)
						continue;
					for (auto &face : bData.GetFacesData())
					{
						int value = static_cast<int>(face.faceDirection);
						Faces opposite = value % 2 == 0 ? static_cast<Faces>(value + 1) : static_cast<Faces>(value - 1);
						auto adjacent = GetAdjacentBlockData(it, face.faceDirection);
						if (!face.isTransparent && adjacent.GetFaceData(opposite).isTransparent)
							this->_blocksDatas[mapIndex]._faces[static_cast<int>(face.faceDirection)].isVisible = true;
						else if (face.isTransparent && adjacent.type != bData.type &&
								 adjacent.GetFaceData(opposite).isTransparent)
							this->_blocksDatas[mapIndex]._faces[static_cast<int>(face.faceDirection)].isVisible = true;
					}
				}
			}
		}
	}

	Vox::Game::Datas::Blocks::BlockType VoxelChunck::GetBlockDatas(const Vector3Uint8 &localPos)
	{
		size_t index = this->GetLocalIndex(localPos);
		return this->_blocksDatas[index].type;
	}

	void VoxelChunck::UpdateBufferObject()
	{
		if (this->isVisible == false && this->_bufferIndex <= Utils::Defines::CHUNCK_AMOUNT)
		{
			if (this->_bufferIndex < Utils::Defines::CHUNCK_AMOUNT)
			{
				World::WorldManager::GetInstance().ReleaseChunkBuffer(this->_bufferIndex);
				this->_isDirty[0] = false;
				this->_isDirty[1] = false;
			}
			this->_bufferIndex = Utils::Defines::CHUNCK_AMOUNT;
			return;
		}
		else if (this->isVisible == true && this->_bufferIndex >= Utils::Defines::CHUNCK_AMOUNT)
		{
			if (this->_bufferIndex >= Utils::Defines::CHUNCK_AMOUNT)
			{
				this->_bufferIndex = World::WorldManager::GetInstance().RequestChunkBuffer();
			}
			if (this->_bufferIndex >= Utils::Defines::CHUNCK_AMOUNT)
			{
				LoggerLib::LogDebug("Trying To Refresh Buffer but too big: ", this->_bufferIndex);
				return;
			}
			this->_isDirty[0] = true;
			this->_isDirty[1] = true;
		}
	}

	size_t VoxelChunck::GetLocalIndex(const Vector3Uint8 &vec)
	{
		return vec[0] + (vec[1] * CHUNCK_SIZE) + (vec[2] * (CHUNCK_SIZE * CHUNCK_SIZE));
	}

	Vector3Uint8 VoxelChunck::GetLocalVector(const size_t &index)
	{
		return Vector3Uint8(
			index % Utils::Defines::CHUNCK_SIZE, (index / Utils::Defines::CHUNCK_SIZE) % Utils::Defines::CHUNCK_SIZE,
			((index / Utils::Defines::CHUNCK_SIZE) / Utils::Defines::CHUNCK_SIZE) % Utils::Defines::CHUNCK_SIZE);
	}

	Game::Datas::Blocks::BlockData &VoxelChunck::GetBlockData(Vector3Uint8 chPos)
	{
		return this->_blocksDatas[this->GetLocalIndex(chPos)];
	}

	// bool VoxelChunck::Render(uint8_t toRender)
	// {
	// if (this->isVisible == false)
	// 	return false;
	// return false;
	// using namespace Front::Rendering;
	// auto frame = SyncObjects::GetInstance().GetCurrentFrame();
	// if (this->_isDirty[frame] == true || this->_bufferIndex >= Utils::Defines::CHUNCK_AMOUNT)
	// 	return true;

	// (void)toRender;
	// uint32_t dynamicOffset = this->_bufferIndex * Utils::Vulkan::GetAlignedChunckSize();

	// auto buffer = CommandsPool::GetInstance().GetBuffer(frame);
	// if (toRender == 0 && this->indexCountOpaque != 0)
	// {
	// auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::VoxelPipeline>("Voxel");
	// if (pipeline == nullptr)
	// return true;
	// VkDeviceSize offset = {0};
	// LoggerLib::LogDebug("Rendering opaque buffer at ", this->_bufferIndex);
	// vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1,
	// &pipeline->GetSet(frame), 1, &dynamicOffset);
	// ;(buffer, 0, 1, &this->_mVertOpaque->buffer->GetBuffer(frame), &offset);
	// vkCmdBindIndexBuffer(buffer, this->_mIndexOpaque->buffer->GetBuffer(frame), 0, VK_INDEX_TYPE_UINT16);
	// vkCmdDrawIndexed(buffer, indexCountOpaque, 1, 0, 0, 0);
	// }
	// else if (toRender == 1 && this->indexCountTransparent != 0)
	// {
	// auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::TransparentVoxelPipeline>(
	// "Voxel_Transparent");
	// if (pipeline == nullptr)
	// return true;
	// VkDeviceSize offset = {0};
	// // LoggerLib::LogDebug("Rendering transparent buffer at ", this->_bufferIndex);
	// vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1,
	// &pipeline->GetSet(frame), 1, &dynamicOffset);
	// vkCmdBindVertexBuffers(buffer, 0, 1, &this->_mVertTransparent->buffer->GetBuffer(frame), &offset);
	// vkCmdBindIndexBuffer(buffer, this->_mIndexTransparent->buffer->GetBuffer(frame), 0, VK_INDEX_TYPE_UINT16);
	// vkCmdDrawIndexed(buffer, indexCountTransparent, 1, 0, 0, 0);
	// }
	// return true;
	// }

	uint16_t VoxelChunck::GetBuffer() const
	{
		return this->_bufferIndex;
	}

	void VoxelChunck::AddFace(const std::vector<Game::Datas::Textures::TextureInfo> &textInfo,
							  std::vector<Engine::Meshs::Vertex> &vert, std::vector<uint16_t> &idx, const Faces &face,
							  const Vector3Uint8 &facePos, const Game::Datas::Blocks::BlockType &blockType,
							  const Vector3Float &faceColor, float faceOffsef)
	{
		std::array<Engine::Meshs::Vertex, 4> toAdd = defaultFacesPos.at(face);

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
		}

		auto beg = vert.end();
		vert.insert(beg, toAdd.begin(), toAdd.end());
		uint32_t size = idx.empty() ? 0 : idx[idx.size() - 1] + 1;
		idx.push_back(size);
		idx.push_back(size + 1);
		idx.push_back(size + 2);
		idx.push_back(size);
		idx.push_back(size + 2);
		idx.push_back(size + 3);
	}

	void VoxelChunck::GEN_BuildMesh()
	{
		Vector3Uint8 it(0);
		Vector3Float color(1.0, 1.0, 1.0);
		const auto &tManagers = Vox::Front::Scenes::TexturesManager::GetInstance();

		std::vector<Engine::Meshs::Vertex> oVert;
		std::vector<Engine::Meshs::Vertex> tVert;

		std::vector<uint16_t> oIdx;
		std::vector<uint16_t> tIdx;

		for (it[0] = 0; it[0] < CHUNCK_SIZE; it[0]++)
		{
			for (it[2] = 0; it[2] < CHUNCK_SIZE; it[2]++)
			{
				for (it[1] = 0; it[1] < CHUNCK_SIZE; it[1]++)
				{
					uint16_t mapIndex = GetLocalIndex(it);
					auto data = this->_blocksDatas[mapIndex];
					if (data.type == Vox::Game::Datas::Blocks::BlockType::Air)
						continue;
					auto faces = data.GetFacesData();

					for (auto face : faces)
					{
						if (face.isVisible == false)
							continue;

						if (face.isTransparent)
							this->AddFace(tManagers.operator[]("A_Blocks_Transparent").GetTextureInfo(), tVert, tIdx,
										  face.faceDirection, it, data.type, color, face.isTransparent ? 0.1 : 0);
					}
				}
			}
		}

		this->_opaqueMesh.SetMeshDatas(oVert, oIdx);
		this->_transparentMesh.SetMeshDatas(tVert, tIdx);
	}

	void VoxelChunck::UpdateVisibility()
	{
		this->isVisible = this->IsOnFrustum(Game::World::WorldManager::GetCamera().GetFrustum());
	}

	VoxelChunck::~VoxelChunck()
	{
		if (this->_bufferIndex < Utils::Defines::CHUNCK_AMOUNT)
		{
			auto index = this->_bufferIndex;
			this->_bufferIndex = Utils::Defines::CHUNCK_AMOUNT;
			World::WorldManager::GetInstance().ReleaseChunkBuffer(index);
		}
	}

	void VoxelChunck::GEN_TerrainData(std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> &bs,
									  const Generation::Utils::ChunckCache &cache)
	{
		Vector3Uint8 it(0);
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
					if (!bs[mapIndex])
					{
						if (h > Generation::Utils::WATER_LEVEL)
						{
							this->_blocksDatas[mapIndex].type = Game::Datas::Blocks::BlockType::Air;
							this->_blocksDatas[mapIndex].UpdateFacesTransparency({true, true, true, true, true, true});
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
						this->_blocksDatas[mapIndex].UpdateFacesTransparency(
							{false, false, false, false, false, false});
					}
				}
			}
		}
	}
} // namespace Vox::Game::World::Chuncks