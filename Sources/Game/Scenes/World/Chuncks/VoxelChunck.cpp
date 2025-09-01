#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"

#include <array>

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

namespace Vox::Game::World::Chuncks
{
	VoxelChunck::VoxelChunck(size_t bufferIndex) : _bufferIndex(bufferIndex)
	{
		this->B_Index = nullptr;
		this->B_Vertex = nullptr;

        BuildVoxelObject();
        BuildMatrix({0.0, 0.0, 0.0});
	}

	void VoxelChunck::BuildVoxelObject()
	{
		std::array<Vertex, 8> vertex = {
			Vertex({0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}), Vertex({0.5, -0.5, 0.5}, {1.0, 1.0, 1.0}),
			Vertex({-0.5, -0.5, 0.5}, {1.0, 1.0, 1.0}), Vertex({-0.5, -0.5, -0.5}, {1.0, 1.0, 1.0}),
			Vertex({0.5, 0.5, -0.5}, {1.0, 1.0, 1.0}),	Vertex({0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}),
			Vertex({-0.5, 0.5, 0.5}, {1.0, 1.0, 1.0}),	Vertex({-0.5, 0.5, -0.5}, {1.0, 1.0, 1.0}),
		};
		std::array<uint16_t, 36> index = {0, 1, 2, 2, 3, 0,
										  // haut
										  4, 5, 6, 6, 7, 4,
										  // devant
										  1, 5, 6, 6, 2, 1,
										  // derrière
										  0, 4, 7, 7, 3, 0,
										  // droite
										  0, 1, 5, 5, 4, 0,
										  // gauche
										  3, 2, 6, 6, 7, 3};

		if (this->B_Index != nullptr)
        {
			delete this->B_Vertex;
			delete this->B_Index;
        }
        this->B_Index = new sbuffer(2, sizeof(index), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
        this->B_Vertex = new sbuffer(2, sizeof(index), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
        this->B_Index->Create(index.data());
	
        this->B_Vertex->Create(vertex.data());
    }
    
    void VoxelChunck::BuildMatrix(Vector3 pos)
    {
        
    }
    
    VoxelChunck::~VoxelChunck() {}
} // namespace Vox::Game::World::Chuncks