#ifndef __VOXELCHUNCK_HPP__
#define __VOXELCHUNCK_HPP__

#include "./BlockData.hpp"
#include "./E_GenerationState.hpp"
#include "./FacesData.hpp"
#include "Front/Rendering/Frustum/BoxCollider.hpp"
#include "Game/Datas/TexturesData.hpp"
#include "Game/Models/DynamicObject.hpp"
#include "Game/Scenes/World/Generation/ThreadObject.hpp"
#include "Game/Scenes/World/Generation/Utils.hpp"
#include "MathGraphicalLib/Matrix/Matrix4.hpp"
#include "MathGraphicalLib/Vectors/Defines.hpp"
#include "Spline/Spline.hpp"
#include <bitset>
#include <unordered_map>

#include "Game/Scenes/World/Generation/BufferMemoryManager.hpp"

#include "Engine/Meshs/VoxelMesh.hpp"

#include "Engine/Rendering/IDrawable.hpp"
#include "Engine/Memory/Align.hpp"

namespace Vox::Game::Scenes::World::Player
{
	class Camera;
}

namespace Vox::Game::World::Chuncks
{
	using namespace Game::Utils::Defines;
	using namespace MGL::Vectors::Types;

	class ChunckCluster;

	class VoxelChunck : public Models::DynamicObject,
						public Front::Rendering::Frustum::Colliders::BoxCollider,
						public Engine::Rendering::IDrawable
	{
		public:
			struct ChunckUniform
			{
					MGL::Matrix::Matrix4 model;
			};

			friend class ChunckCluster;

			// bool Render(uint8_t toRender);

			bool Draw() override;

			void UpdateBufferObject();

#pragma region Getters
			Vector3Int GetChunckPosition()
			{
				return this->_chunckPos;
			};

			uint16_t GetBuffer() const;
			static size_t GetLocalIndex(const Vector3Uint8 &vec);
			static Vector3Uint8 GetLocalVector(const size_t &index);

			Game::Datas::Blocks::BlockData &GetBlockData(Vector3Uint8);
#pragma endregion

#pragma region Setters
#pragma endregion

#pragma region Constructors
			VoxelChunck() = delete;
			VoxelChunck(const Vector3Int &defaultPos = {0, 0, 0});
			~VoxelChunck();
#pragma endregion

		private:
#pragma region Variables
			bool isVisible = false;

			std::array<Game::Datas::Blocks::BlockData, CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> _blocksDatas;

			Engine::Meshs::VoxelMesh _opaqueMesh;
			Engine::Meshs::VoxelMesh _transparentMesh;

			Vector3Int _chunckPos;
			size_t _bufferIndex;
#pragma endregion

#pragma region Generation

			void GEN_TerrainData(std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> &,
								 const Generation::Utils::ChunckCache &);
			void GEN_BuildMesh();
			void GEN_FacesCulling(const Generation::Utils::ChunckCache &cache);
			std::unique_ptr<std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE>> GEN_ContentBitset(
				const uint8_t hMap[Generation::Utils::CACHE_SIZE * Generation::Utils::CACHE_SIZE]);

#pragma endregion

			bool AssignModel() override;

			void SetBlockDatas(const Vector3Uint8 &, Vox::Game::Datas::Blocks::BlockType);

			Vox::Game::Datas::Blocks::BlockType GetBlockDatas(const Vector3Uint8 &localPos);

			void AddFace(const std::vector<Game::Datas::Textures::TextureInfo> &, std::vector<Engine::Meshs::Vertex> &,
						 std::vector<uint16_t> &, const Faces &, const Vector3Uint8 &,
						 const Game::Datas::Blocks::BlockType &, const Vector3Float &, float);

			void UpdateVisibility();

			/* private */
	};
} // namespace Vox::Game::World::Chuncks

#endif // __VOXELCHUNCK_HPP__