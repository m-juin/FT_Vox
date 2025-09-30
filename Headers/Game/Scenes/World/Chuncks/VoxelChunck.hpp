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

#include "Game/Utils/TexturesData.hpp"

namespace Vox::Game::World::Chuncks
{
	using namespace Game::Utils::Defines;

	class VoxelChunck : public Models::DynamicObject
	{
		public:
			struct ChunckUniform
			{
					MGL::Matrix::Matrix4 model;
			};

			using LocalVector = MGL::Vectors::Vector3<uint8_t>;
			VoxelChunck() = delete;
			VoxelChunck(const Vector3Int &defaultPos = {0, 0, 0});
			~VoxelChunck();

			void Render();
			void BuildVoxelObject(const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl,
								  const std::vector<Game::Utils::Textures::TextureInfo> &textInfo,
								  const uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE], const uint32_t &seed);
			void BuildBufferObject(const uint16_t &buffer);

			Vector3Int GetChunckPosition()
			{
				return this->_chunckPos;
			};

			uint16_t GetBuffer() const;

		private:
			std::vector<uint16_t> index;
			std::vector<Vertex> vertex;
			uint16_t indexCount;
			Vector3Int _chunckPos;

			void AddFace(const std::vector<Game::Utils::Textures::TextureInfo> &textInfo, const Faces &face,
						 const LocalVector &facePos, const std::string &blockType,
						 const Vector3Float &faceColor = {1.0, 1.0, 1.0});
			void AssignModel() override;
			static size_t GetLocalIndex(const LocalVector &vec);
			static LocalVector GetLocalVector(const size_t &index);
			std::bitset<CHUNCK_SIZE * CHUNCK_SIZE * CHUNCK_SIZE> BuildContent(
				const uint8_t hMap[CHUNCK_SIZE * CHUNCK_SIZE]);

			size_t _bufferIndex;
			sbuffer *B_Vertex;
			sbuffer *B_Index;

			/* private */
	};
} // namespace Vox::Game::World::Chuncks

#endif // __VOXELCHUNCK_HPP__