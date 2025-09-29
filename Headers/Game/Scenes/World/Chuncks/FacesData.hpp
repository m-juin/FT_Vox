#ifndef __FACESDATA_HPP__
#define __FACESDATA_HPP__

#include <unordered_map>
#include "Game/Scenes/World/Utils/Defines.hpp"

namespace Vox::Game::World::Chuncks
{
	using namespace Utils::Defines;
	enum class Faces
	{
		TOP,
		BOT,
		LEFT,
		RIGHT,
		FRONT,
		BACK,
		NONE
	};
	const std::unordered_map<Faces, std::array<Vertex, 4>> defaultFacesPos = {
		{Faces::TOP, std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
										   Vertex{Vector3Float(-0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
										   Vertex{Vector3Float(-0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
										   Vertex{Vector3Float(0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1}}},
		{Faces::BOT, std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
										   Vertex{Vector3Float(-0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
										   Vertex{Vector3Float(-0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
										   Vertex{Vector3Float(0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1}}},
		{Faces::LEFT, std::array<Vertex, 4>{Vertex{Vector3Float(-0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											Vertex{Vector3Float(-0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											Vertex{Vector3Float(-0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											Vertex{Vector3Float(-0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1}}},
		{Faces::RIGHT, std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											 Vertex{Vector3Float(0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											 Vertex{Vector3Float(0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											 Vertex{Vector3Float(0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1}}},
		{Faces::FRONT, std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											 Vertex{Vector3Float(-0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											 Vertex{Vector3Float(-0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											 Vertex{Vector3Float(0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), -1}}},
		{Faces::BACK, std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											Vertex{Vector3Float(-0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											Vertex{Vector3Float(-0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1},
											Vertex{Vector3Float(0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), -1}}},
	};
} // namespace Vox::Game::World::Chuncks

#endif // __FACESDATA_HPP__