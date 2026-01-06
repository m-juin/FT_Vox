#include "Game/Scenes/World/Chuncks/ChunckCluster.hpp"

#include "Game/Scenes/World/Generation/PerlinInterpretation.hpp"

#include "Game/Scenes/World/Generation/GenerationManager.hpp"
#include "Game/Scenes/World/Generation/PerlinUtils.hpp"
#include "Game/Scenes/World/WorldManager.hpp"

#include "Game/Scenes/World/Generation/Decorations/PoissonDiskSampling.hpp"

#include "Game/Datas/Structures/StructuresManager.hpp"

#include "Game/Scenes/World/Chuncks/Operations.hpp"
#include "Game/Scenes/World/Generation/ChunkOverflowManager.hpp"

#include "Utils/TracyUtils.hpp"

namespace Vox::Game::World::Chuncks
{
	Game::Generation::Utils::ChunckCache ChunckCluster::GenerateCache(
		const uint32_t seed, const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl)
	{
		Game::Generation::Utils::ChunckCache cacheSt;

		int baseX = static_cast<int>(this->_clusterPos[0] * CHUNCK_SIZE);
		int baseZ = static_cast<int>(this->_clusterPos[1] * CHUNCK_SIZE);

		for (int x = -Game::Generation::Utils::GENERATION_BLEND_RADIUS;
			 x < static_cast<int>(CHUNCK_SIZE) + Game::Generation::Utils::GENERATION_BLEND_RADIUS; x++)
		{
			for (int z = -Game::Generation::Utils::GENERATION_BLEND_RADIUS;
				 z < static_cast<int>(CHUNCK_SIZE) + Game::Generation::Utils::GENERATION_BLEND_RADIUS; z++)
			{
				size_t localIndex =
					(x + Game::Generation::Utils::GENERATION_BLEND_RADIUS) * Game::Generation::Utils::CACHE_SIZE +
					(z + Game::Generation::Utils::GENERATION_BLEND_RADIUS);

				int worldX = baseX + x;
				int worldZ = baseZ + z;

				auto i = Generation::Perlins::GetBiomeInfoAtPoint(worldX, worldZ, seed);

				cacheSt.continental[localIndex] = i.Continental;
				cacheSt.erosion[localIndex] = i.Erosion;
				cacheSt.peaks[localIndex] = i.PV;
				cacheSt.biome[localIndex] = i.Biome;
			}
		}
		for (int x = -Game::Generation::Utils::GENERATION_BLEND_RADIUS;
			 x < static_cast<int>(CHUNCK_SIZE) + Game::Generation::Utils::GENERATION_BLEND_RADIUS; x++)
		{
			for (int z = -Game::Generation::Utils::GENERATION_BLEND_RADIUS;
				 z < static_cast<int>(CHUNCK_SIZE) + Game::Generation::Utils::GENERATION_BLEND_RADIUS; z++)
			{
				size_t localIndex =
					(x + Game::Generation::Utils::GENERATION_BLEND_RADIUS) * Game::Generation::Utils::CACHE_SIZE +
					(z + Game::Generation::Utils::GENERATION_BLEND_RADIUS);
				cacheSt.heightMap[localIndex] = Generation::Perlins::GetBlendedCached(x, z, spl, cacheSt);
			}
		}
		return cacheSt;
	}

	std::pair<size_t, size_t> ChunckCluster::Render(uint8_t toRender)
	{
		size_t rendered = 0;
		size_t tried = 0;
		// if (this->GetGenerationState() != Generation::E_GenerationState::End)
		// 	return {0, 0};
		for (auto ch : this->_clusterContent)
		{
			if (ch)
			{
				if (ch->Render(toRender))
					rendered++;
				tried++;
			}
		}
		return {tried, rendered};
	}

	void ChunckCluster::GenerateHeightMap(
		const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl, const uint32_t seed,
		uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE])
	{
		int baseX = static_cast<int>(this->_clusterPos[0] * CHUNCK_SIZE);
		int baseZ = static_cast<int>(this->_clusterPos[1] * CHUNCK_SIZE);

		for (size_t x = 0; x < CHUNCK_SIZE; x++)
		{
			for (size_t z = 0; z < CHUNCK_SIZE; z++)
			{
				hMap[x * CHUNCK_SIZE + z] = Generation::Perlins::GetHeightAt(baseX + x, baseZ + z, seed, spl);
			}
		}
	}

	void ChunckCluster::BuildClusterContent(
		const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl,
		const std::vector<Game::Datas::Textures::TextureInfo> &textInfo,
		const std::vector<Game::Datas::Textures::TextureInfo> &transparenttextInfo, const uint32_t seed)
	{
		(void)textInfo;
		(void)transparenttextInfo;
		this->ChangeGenerationState(Generation::E_GenerationState::Mesh);

		auto st = GenerateCache(seed, spl);

		int chunksPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;
		for (int y = chunksPerCluster - 1; y >= 0; y--)
		{
			if (this->IsGenerationCancelled())
				return;
			this->_clusterContent[y] = new VoxelChunck(Vector3Int(this->_clusterPos[0], y, this->_clusterPos[1]));

			this->_clusterContent[y]->BuildVoxelObject(st, seed);
		}
		if (this->IsGenerationCancelled())
			return;
		for (int y = chunksPerCluster - 1; y >= 0; y--)
		{
			if (this->IsGenerationCancelled())
				return;
			this->_clusterContent[y]->BuildMesh();
		}
		if (this->IsGenerationCancelled())
			return;
		this->GenerateClusterDecoration(st, spl, seed);
		this->ChangeGenerationState(Generation::E_GenerationState::WaitingBuffer);
	}

	void ChunckCluster::GenerateTree(const Vox::Game::Generation::Utils::ChunckCache &cache, const uint32_t seed)
	{
		std::seed_seq seed_seq{seed, static_cast<uint32_t>(_clusterPos[0]), static_cast<uint32_t>(_clusterPos[1])};
		thread_local std::mt19937 generator(seed_seq);
		std::uniform_int_distribution<int> distribution(0, 100);
		size_t regionSeed = Vox::World::Generation::Decorations::GetRegionnedSeed(seed, this->_clusterPos, 2);
		auto trees =
			Vox::World::Generation::Decorations::GetChunckDiskSampling(this->_clusterPos, regionSeed, 4.0, 2, 10);
		std::map<Game::Datas::Biomes::Biomes, uint8_t> counters;
		for (auto treePos : trees)
		{
			int localX = treePos[0];
			int localZ = treePos[1];

			int cacheX = localX + Game::Generation::Utils::GENERATION_BLEND_RADIUS;
			int cacheZ = localZ + Game::Generation::Utils::GENERATION_BLEND_RADIUS;

			size_t arrayIndex = cacheX * Game::Generation::Utils::CACHE_SIZE + cacheZ;
			auto biome = cache.biome[arrayIndex];
			auto biomeDensity = Vox::Game::Generation::Datas::Biomes::RulesManager::GetTreeChance(biome);
			if (biomeDensity == 0)
				continue;
			size_t worldHeight = cache.heightMap[arrayIndex];
			if (worldHeight < Generation::Utils::WATER_LEVEL)
				continue;
			auto it = counters.find(biome);
			if (it == counters.end())
			{
				counters[biome] = 1;
				it = counters.find(biome);
			}
			else
				it->second += 1;
			if ((double)it->second >= 10 - biomeDensity / 10)
				it->second = 0;
			if (it->second == 0.0)
			{
				int val = distribution(generator);
				auto type = Vox::Game::Generation::Datas::Biomes::RulesManager::GetTreeType(biome, val);
				if (type == Game::Datas::Structures::StructuresType::None)
					continue;
				this->SpawnStructure(type, {static_cast<uint8_t>(treePos[0]), static_cast<uint8_t>(worldHeight + 1),
											static_cast<uint8_t>(treePos[1])});
			}
		}
	}

	void ChunckCluster::GenerateClusterDecoration(
		const Vox::Game::Generation::Utils::ChunckCache &cache,
		const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl, const uint32_t seed)
	{
		(void)spl;
		(void)cache;
		(void)seed;
		GenerateTree(cache, seed);
		GetOverflowBlocks();
	}

	void ChunckCluster::SpawnStructure(Game::Datas::Structures::StructuresType type, Vector3Int pos)
	{
		auto &gManager = Game::World::WorldManager::GetInstance().GetGenerationManager();

		auto s = gManager.GetStructuresManager()->GetStructure(type);
		auto oManager = gManager.GetOverflowManager();

		auto content = s.GetContent();
		Vector3Int sSize = {static_cast<int>(s._structureSize[0]), static_cast<int>(s._structureSize[1]),
							static_cast<int>(s._structureSize[2])};

		Vector3Int sPos{};
		std::unordered_map<Vox::Game::Generation::Vector2Int, std::vector<Game::Generation::ChunkOverflowBlock>,
						   MGL::Vectors::Vector2Hash<int>>
			overflowContent;
		for (sPos[0] = 0; sPos[0] < sSize[0]; sPos[0]++)
		{
			for (sPos[1] = 0; sPos[1] < sSize[1]; sPos[1]++)
			{
				for (sPos[2] = 0; sPos[2] < sSize[2]; sPos[2]++)
				{
					auto bType = content[s.GetLocalIndex(sPos)];
					Vector3Int oPos = {static_cast<int>(sPos[0] - s._anchorPoint[0] + pos[0]),
									   static_cast<int>(sPos[1] - s._anchorPoint[1] + pos[1]),
									   static_cast<int>(sPos[2] - s._anchorPoint[2] + pos[2])};
					if (bType == Game::Datas::Blocks::BlockType::Air)
						continue;
					auto chunckIndex = oPos[1] / CHUNCK_SIZE;
					float localHeight = oPos[1] % CHUNCK_SIZE;

					if (oPos[0] < 0 || oPos[0] >= static_cast<int>(CHUNCK_SIZE) || oPos[2] < 0 ||
						static_cast<int>(oPos[2] >= static_cast<int>(CHUNCK_SIZE)))
					{
						Vector3Int offset;
						Vector3Int worldPos = {static_cast<int>((this->_clusterPos[0] * CHUNCK_SIZE) + oPos[0]),
											   oPos[1],
											   static_cast<int>((this->_clusterPos[1] * CHUNCK_SIZE) + oPos[2])};

						Game::Generation::ChunkOverflowBlock block = {{static_cast<int>(worldPos[0]),
																	   static_cast<int>(worldPos[1]),
																	   static_cast<int>(worldPos[2])},
																	  bType};
						overflowContent[Game::Chuncks::Operations::WorldToCluster(worldPos)].push_back(block);
						continue;
					}
					this->_clusterContent[chunckIndex]->SetBlockDatas({static_cast<uint8_t>(oPos[0]),
																	   static_cast<uint8_t>(localHeight),
																	   static_cast<uint8_t>(oPos[2])},
																	  bType, false);
				}
			}
		}
		for (auto clusterPos : overflowContent)
		{
			oManager->AddBlocks(clusterPos.first, clusterPos.second);
		}
	}
	void ChunckCluster::GetOverflowBlocks()
	{
		auto &gManager = Game::World::WorldManager::GetInstance().GetGenerationManager();

		auto oManager = gManager.GetOverflowManager();
		auto blocks = oManager->ExtractClusterBlocks(this->_clusterPos);
		for (auto block : blocks)
		{
			auto chunckIndex = block.worldCoord[1] / CHUNCK_SIZE;
			const Vox::Game::World::Chuncks::VoxelChunck::LocalVector localPos =
				Game::Chuncks::Operations::WorldToChunk(block.worldCoord);
			if (this->_clusterContent[chunckIndex]->GetBlockDatas(localPos) != Vox::Game::Datas::Blocks::BlockType::Air)
				continue;
			this->_clusterContent[chunckIndex]->SetBlockDatas(localPos, block.type, false);
		}
	}

	void ChunckCluster::UpdateClusterIfNeeded()
	{
		auto &gManager = Game::World::WorldManager::GetInstance().GetGenerationManager();
		auto oManager = gManager.GetOverflowManager();
		auto blocks = oManager->ExtractClusterBlocks(this->_clusterPos);

		if (blocks.size() == 0)
			return;

		std::array<std::unordered_map<Vector3Uint8, Game::Datas::Blocks::BlockType,
									  MGL::Vectors::Vector3Hash<uint8_t>>,
				   WORLD_HEIGHT / CHUNCK_SIZE>
			localMap;
		for (auto block : blocks)
		{
			auto chunckIndex = block.worldCoord[1] / CHUNCK_SIZE;
			localMap[chunckIndex][Game::Chuncks::Operations::WorldToChunk(block.worldCoord)] = block.type;
		}
		for (size_t cIndex = 0; cIndex < localMap.size(); cIndex++)
		{
			auto array = localMap[cIndex];
			for (size_t bIndex = 0; bIndex < array.size(); bIndex++)
			{
				std::unordered_map<Vector3Uint8, Game::Datas::Blocks::BlockType,
								   MGL::Vectors::Vector3Hash<uint8_t>>::iterator it = array.begin();
				std::advance(it, bIndex);
				this->_clusterContent[cIndex]->SetBlockDatas(it->first, it->second, bIndex == array.size() - 1);
			}
		}
	}

	Vector2Int ChunckCluster::GetPosition()
	{
		return this->_clusterPos;
	}

	void ChunckCluster::SetBlock(MGL::Vectors::Vector3<uint8_t> localPos, Game::Datas::Blocks::BlockType type)
	{
		(void)localPos;
		(void)type;
	}

	void ChunckCluster::BuildBuffers()
	{
		int chunksPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;
		for (int y = chunksPerCluster - 1; y >= 0; y--)
		{
			// int localIndex = chunksPerCluster - 1 - y;
			// int globalIndex = _bufferIndex * chunksPerCluster + localIndex;
			auto ch = this->_clusterContent[y];
			if (ch)
				ch->UpdateBufferObject();
		}
		this->_currentState = Generation::E_GenerationState::End;
	}

	ChunckCluster::ChunckCluster(const Vector2Int &coord) : _clusterContent{}
	{
		_clusterPos = coord;
		this->onUpdate.AddCallBack(
			[this]()
			{
				// if (this->GetGenerationState() != Vox::Game::Generation::E_GenerationState::End)
				// 	return;

				{
#ifdef TRACY_ENABLE
					ZoneScopedNC("Update cluster if Needed", tracy::Color::Yellow1);
#endif
					this->UpdateClusterIfNeeded();
				}

				for (auto ch : this->_clusterContent)
				{
					if (ch)
					{
#ifdef TRACY_ENABLE
						ZoneScopedNC("Chunk Update", tracy::Color::Green1);
						// auto chunkY = ;
						ZoneValue(ch->GetChunckPosition()[1]);
#endif
						ch->Update();
					}
				}
			});
		this->_currentState = Generation::E_GenerationState::WaitingThread;
	}

	ChunckCluster::~ChunckCluster()
	{
		for (auto ch : this->_clusterContent)
		{
			if (ch)
				delete ch;
		}
	}
} // namespace Vox::Game::World::Chuncks
