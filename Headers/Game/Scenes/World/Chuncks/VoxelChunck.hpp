#ifndef __VOXELCHUNCK_HPP__
#define __VOXELCHUNCK_HPP__

#include "MathGraphicalLib/Matrix/Matrix4.hpp"

#include "Game/Scenes/World/Utils/Defines.hpp"

#include "./E_GenerationState.hpp"
#include "Game/Models/DynamicObject.hpp"

#include <unordered_map>

#include "Game/Scenes/World/Generation/ThreadObject.hpp"

#include "./FacesData.hpp"

#include "Spline/Spline.hpp"

#include <bitset>

#include "Game/Datas/TexturesData.hpp"

#include "Game/Scenes/World/Generation/Utils.hpp"

#include "./BlockData.hpp"

#include "Front/Rendering/Frustum/BoxCollider.hpp"

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
			std::bitset<2> _needbufferUpdate;
			VoxelChunck() = delete;
			VoxelChunck(const Vector3Int &defaultPos = {0, 0, 0});
			~VoxelChunck();

			bool Render(uint8_t toRender);
			void BuildVoxelObject(const Generation::Utils::ChunckCache &cache, const uint32_t &seed);
			void BuildBufferObject();

			Vector3Int GetChunckPosition()
			{
				return this->_chunckPos;
			};

			uint16_t GetBuffer() const;
			static size_t GetLocalIndex(const LocalVector &vec);
			static LocalVector GetLocalVector(const size_t &index);

		private:
			bool isVisible = false;

			std::array<Game::Datas::Blocks::BlockData, CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> _blocksDatas;
			std::vector<uint16_t> indexOpaque;
			std::vector<Vertex> vertexOpaque;
			std::vector<uint16_t> indexTransparent;
			std::vector<Vertex> vertexTransparent;

			uint16_t indexCountOpaque;
			uint16_t indexCountTransparent;
			Vector3Int _chunckPos;
			void AddFace(const std::vector<Game::Datas::Textures::TextureInfo> &textInfo, const Faces &face,
						 const LocalVector &facePos, const Game::Datas::Blocks::BlockType &blockType,
						 const Vector3Float &faceColor = {1.0, 1.0, 1.0}, bool target = 0, float faceOffsef = 1.0);
			void AssignModel() override;
			std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> BuildContent(
				const uint8_t hMap[Generation::Utils::CACHE_SIZE * Generation::Utils::CACHE_SIZE]);

			void SetBlocksDatas(const Generation::Utils::ChunckCache &cache,
								std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> &clusterContent);
			void SetBlockDatas(const LocalVector &, Vox::Game::Datas::Blocks::BlockType, bool = false);
			void FacesCulling(const Generation::Utils::ChunckCache &cache);
			Vox::Game::Datas::Blocks::BlockType GetBlockDatas(const LocalVector &localPos);

			void BuildMesh();
			size_t _bufferIndex;
			dbuffer *B_VertexOpaque = nullptr;
			dbuffer *B_IndexOpaque = nullptr;
			dbuffer *B_VertexTransparent = nullptr;
			dbuffer *B_IndexTransparent = nullptr;
			void RefreshBuffers();
			void DeleteBuffers();

			/* private */
	};
} // namespace Vox::Game::World::Chuncks

#endif // __VOXELCHUNCK_HPP__