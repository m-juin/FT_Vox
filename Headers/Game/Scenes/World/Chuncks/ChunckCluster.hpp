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

#include "Engine/Rendering/Buffers/Buffer.hpp"
#include "Engine/Rendering/IDrawable.hpp"

namespace Vox::Game::World::Chuncks
{
	using namespace MGL::Vectors::Types;
	const uint8_t ChunkPerCluster = WORLD_HEIGHT / CHUNCK_SIZE;

	class ChunckCluster : public Generation::Threads::ThreadObject, public Vox::Utils::AUpdatable, public Engine::Rendering::IDrawable
	{
		private:
#pragma region Constructor
			ChunckCluster() = delete;
#pragma endregion

#pragma region Variables
			std::array<std::unique_ptr<VoxelChunck>, WORLD_HEIGHT / CHUNCK_SIZE> _clusterContent;
			Vector2Int _clusterPos;

			std::unique_ptr<Engine::Rendering::Buffers::Buffer> _opaqueVertexBuffer;
			std::unique_ptr<Engine::Rendering::Buffers::Buffer> _opaqueIndexBuffer;
			std::unique_ptr<Engine::Rendering::Buffers::Buffer> _transparentVertexBuffer;
			std::unique_ptr<Engine::Rendering::Buffers::Buffer> _transparentIndexBuffer;
#pragma endregion

#pragma region Generation
			Game::Generation::Utils::ChunckCache GEN_Cache(
				const uint32_t, const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &);
			void GEN_CreateChunks();

			std::vector<std::unique_ptr<std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE>>> GEN_TerrainShape(
				const Vox::Game::Generation::Utils::ChunckCache &);
			void GEN_TerrainDatas(std::vector<std::unique_ptr<std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE>>> &,
								  const Vox::Game::Generation::Utils::ChunckCache &);
			void GEN_FacesCulling(const Vox::Game::Generation::Utils::ChunckCache &);
			void GEN_Mesh();

			void GEN_TerrainDecoration(const Vox::Game::Generation::Utils::ChunckCache &, const uint32_t);
			void DGEN_Tree(const Vox::Game::Generation::Utils::ChunckCache &, const uint32_t);
			void DGEN_SpawnStruct(Game::Datas::Structures::StructuresType, Vector3Int);
			void DGEN_Overflow();

#pragma endregion

			void UpdateClusterIfNeeded();

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
			bool Draw() override;
#pragma endregion

#pragma region Generation
			void GEN_Generate(const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &,
							  const uint32_t);
#pragma endregion

#pragma region Constructor
			ChunckCluster(const Vector2Int &);
			~ChunckCluster();
#pragma endregion

#pragma region Memory

	void MEM_CreateBuffers();
	void MEM_FillBuffers();

	void MEM_ClearBuffers(uint8_t);
#pragma endregion

};

} // namespace Vox::Game::World::Chuncks
#endif //__CHUNCKCLUSTER_HPP__
