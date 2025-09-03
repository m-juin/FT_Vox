#ifndef __VOXELCHUNCK_HPP__
#define __VOXELCHUNCK_HPP__

#include "MathGraphicalLib/Matrix/Matrix4.hpp"

#include "Game/Scenes/World/Utils/Defines.hpp"

#include "./E_FaceDir.hpp"
#include "Game/Models/DynamicObject.hpp"

namespace Vox::Game::World::Chuncks
{
	using namespace Game::Utils::Defines;

	const std::unordered_map<Faces, std::array<Vertex, 4>> defaultFacesPos = {
		{Faces::TOP, std::array<Vertex, 4>{Vertex{Vector3(0.5f, 0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
										   Vertex{Vector3(-0.5f, 0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
										   Vertex{Vector3(-0.5f, 0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
										   Vertex{Vector3(0.5f, 0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)}}},
		{Faces::BOT, std::array<Vertex, 4>{Vertex{Vector3(0.5f, -0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
										   Vertex{Vector3(-0.5f, -0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
										   Vertex{Vector3(-0.5f, -0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
										   Vertex{Vector3(0.5f, -0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)}}},
		{Faces::LEFT, std::array<Vertex, 4>{Vertex{Vector3(-0.5f, -0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
											Vertex{Vector3(-0.5f, -0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
											Vertex{Vector3(-0.5f, 0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
											Vertex{Vector3(-0.5f, 0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)}}},
		{Faces::RIGHT, std::array<Vertex, 4>{Vertex{Vector3(0.5f, -0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
											 Vertex{Vector3(0.5f, -0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
											 Vertex{Vector3(0.5f, 0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
											 Vertex{Vector3(0.5f, 0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)}}},
		{Faces::FRONT, std::array<Vertex, 4>{Vertex{Vector3(0.5f, -0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
											 Vertex{Vector3(-0.5f, -0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
											 Vertex{Vector3(-0.5f, 0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)},
											 Vertex{Vector3(0.5f, 0.5f, 0.5f), Vector3(1.0, 1.0, 1.0)}}},
		{Faces::BACK, std::array<Vertex, 4>{Vertex{Vector3(0.5f, -0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
											Vertex{Vector3(-0.5f, -0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
											Vertex{Vector3(-0.5f, 0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)},
											Vertex{Vector3(0.5f, 0.5f, -0.5f), Vector3(1.0, 1.0, 1.0)}}},
	};
	
    
    class VoxelChunck : public Models::DynamicObject
	{
		public:
			struct ChunckUniform
			{
					MGL::Matrix::Matrix4 model;
			};

            using LocalVector = MGL::Vectors::Vector3<uint8_t>;
			VoxelChunck() = delete;
			VoxelChunck(size_t bufferIndex, Vector3 defaultPos = {0.0, 0.0, 0.0});
			~VoxelChunck();

			void Render();

		private:
            size_t indexCount;

            void AddFace(const Faces &face, const LocalVector &facePos, std::vector<Vertex> &vertex, std::vector<uint16_t> &index);
            void AssignModel() override;
            void BuildVoxelObject();
            size_t GetLocalIndex(const LocalVector &vec);
            LocalVector GetLocalVector(const size_t &index);

			size_t _bufferIndex;
			sbuffer *B_Vertex;
			sbuffer *B_Index;

			/* private */
	};
} // namespace Vox::Game::World::Chuncks

#endif // __VOXELCHUNCK_HPP__