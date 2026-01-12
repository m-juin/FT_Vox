#ifndef __CHUNCKCLUSTER_HPP__
#define __CHUNCKCLUSTER_HPP__

#include "../Utils/Defines.hpp"
#include "Game/Scenes/World/Generation/ThreadObject.hpp"

#include <array>

#include "Game/Datas/TexturesData.hpp"
#include "VoxelChunck.hpp"

#include "Game/Datas/Structures/StructuresTypes.hpp"
#include "Game/Scenes/World/Generation/Utils.hpp"

#include "MathGraphicalLib/Vectors/Defines.hpp"

namespace Vox::Game::World::Chuncks
{
	using namespace MGL::Vectors::Types;
	const uint8_t ChunkPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;

	class ChunckCluster : public Generation::Threads::ThreadObject, public Vox::Utils::AUpdatable
	{
		private:
#pragma region Constructor
			ChunckCluster() = delete;
#pragma endregion

#pragma region Variables
			std::array<std::unique_ptr<VoxelChunck>, WORLD_HEIGHT / CHUNCK_SIZE> _clusterContent;
			Vector2Int _clusterPos;
#pragma endregion

			void UpdateClusterIfNeeded();

#pragma region Generation
			Game::Generation::Utils::ChunckCache GEN_Cache(
				const uint32_t,
				const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &);
			void GEN_CreateChunks();

			std::vector<std::unique_ptr<std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE>>> GEN_TerrainShape(const Vox::Game::Generation::Utils::ChunckCache &);
			void GEN_TerrainDatas(std::vector<std::unique_ptr<std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE>>> &, const Vox::Game::Generation::Utils::ChunckCache &);
			void GEN_FacesCulling(const Vox::Game::Generation::Utils::ChunckCache &);
			void GEN_Mesh();

			void GEN_TerrainDecoration(const Vox::Game::Generation::Utils::ChunckCache &, const uint32_t);
			void DGEN_Tree(const Vox::Game::Generation::Utils::ChunckCache &, const uint32_t);
			void DGEN_SpawnStruct(Game::Datas::Structures::StructuresType, Vector3Int);
			void DGEN_Overflow();

#pragma endregion

		public:
#pragma region Getters
			VoxelChunck *GetChunkFromWorld(uint8_t);
			VoxelChunck *GetChunkFromlocal(uint8_t);
			Vector2Int GetPosition();
#pragma endregion

#pragma region Setters
			void SetBlock(MGL::Vectors::Vector3<uint8_t>, Game::Datas::Blocks::BlockType);
			void SetBlocks(
				std::unordered_map<Vector3Int, Game::Datas::Blocks::BlockType, MGL::Vectors::Vector3Hash<int>> &, bool);
#pragma endregion

#pragma region Other
			std::pair<size_t, size_t> Render(uint8_t);
			void BuildBuffers();
#pragma endregion

#pragma region Generation
			void GEN_Generate(const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &,
									 const uint32_t);
#pragma endregion

#pragma region Constructor
			ChunckCluster(const Vector2Int &);
			~ChunckCluster();
#pragma endregion
	};
} // namespace Vox::Game::World::Chuncks
#endif //__CHUNCKCLUSTER_HPP__
