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
#include "Spline/Spline.hpp"
#include <bitset>
#include <unordered_map>

#include "Game/Scenes/World/Generation/BufferMemoryManager.hpp"

#include "Engine/Meshs/VoxelMesh.hpp"

namespace Vox::Game::Scenes::World::Player
{
	class Camera;
}

namespace Vox::Game::World::Chuncks
{
	using namespace Game::Utils::Defines;

	class ChunckCluster;

	class VoxelChunck : public Models::DynamicObject, public Front::Rendering::Frustum::Colliders::BoxCollider
	{
		public:
			struct ChunckUniform
			{
					MGL::Matrix::Matrix4 model;
			};

			friend class ChunckCluster;

			using LocalVector = MGL::Vectors::Vector3<uint8_t>;
			VoxelChunck() = delete;
			VoxelChunck(const Vector3Int &defaultPos = {0, 0, 0});
			~VoxelChunck();

			bool Render(uint8_t toRender);
			void BuildVoxelObject(const Generation::Utils::ChunckCache &cache);
			void UpdateBufferObject();

			Vector3Int GetChunckPosition()
			{
				return this->_chunckPos;
			};

			uint16_t GetBuffer() const;
			static size_t GetLocalIndex(const LocalVector &vec);
			static LocalVector GetLocalVector(const size_t &index);

			Game::Datas::Blocks::BlockData &GetBlockData(Vector3Uint8);

		private:
			bool isVisible = false;

			std::array<Game::Datas::Blocks::BlockData, CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> _blocksDatas;

			Engine::Meshs::VoxelMesh _opaqueMesh;
			Engine::Meshs::VoxelMesh _transparentMesh;

			Vector3Int _chunckPos;

			bool AssignModel() override;

			std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> BuildContent(
				const uint8_t hMap[Generation::Utils::CACHE_SIZE * Generation::Utils::CACHE_SIZE]);

			void SetBlocksDatas(const Generation::Utils::ChunckCache &cache,
								std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> &clusterContent);

			void SetBlockDatas(const LocalVector &, Vox::Game::Datas::Blocks::BlockType);

			void FacesCulling(const Generation::Utils::ChunckCache &cache);

			Vox::Game::Datas::Blocks::BlockType GetBlockDatas(const LocalVector &localPos);

			void AddFace(const std::vector<Game::Datas::Textures::TextureInfo> &, std::vector<Engine::Meshs::Vertex> &,
						 std::vector<uint16_t> &, const Faces &, const LocalVector &,
						 const Game::Datas::Blocks::BlockType &, const Vector3Float &, float);

			void BuildMesh();
			size_t _bufferIndex;

			void UpdateVisibility();

			/* private */
	};
} // namespace Vox::Game::World::Chuncks

#endif // __VOXELCHUNCK_HPP__