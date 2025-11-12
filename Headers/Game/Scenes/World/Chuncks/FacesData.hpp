#ifndef __FACESDATA_HPP__
#define __FACESDATA_HPP__

#include "Game/Scenes/World/Utils/Defines.hpp"
#include <unordered_map>

namespace Vox::Game::World::Chuncks
{
	using namespace Utils::Defines;
	enum class Faces
	{
		TOP = 0,
		BOT = 1,
		LEFT = 2,
		RIGHT = 3,
		FRONT = 4,
		BACK = 5,
		NONE = 6
	};
	constexpr std::string_view faceNames[] = {"Top", "Bot", "Left", "Right", "Front", "Back", ""};
	constexpr std::string_view ToString(Faces face)
	{
		auto idx = static_cast<int>(face);
		if (idx < 0 || idx >= static_cast<int>(Faces::NONE) + 1)
			return "";
		return faceNames[idx];
	}
	const std::unordered_map<Faces, std::array<Vertex, 4>> defaultFacesPos = {
		{Faces::TOP,
		 std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 0.0f}},
							   Vertex{Vector3Float(-0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 0.0f}},
							   Vertex{Vector3Float(-0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 1.0f}},
							   Vertex{Vector3Float(0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 1.0f}}}},
		{Faces::BOT,
		 std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 0.0f}},
							   Vertex{Vector3Float(-0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 0.0f}},
							   Vertex{Vector3Float(-0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 1.0f}},
							   Vertex{Vector3Float(0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 1.0f}}}},
		{Faces::LEFT,
		 std::array<Vertex, 4>{Vertex{Vector3Float(-0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 1.0f}},
							   Vertex{Vector3Float(-0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 1.0f}},
							   Vertex{Vector3Float(-0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 0.0f}},
							   Vertex{Vector3Float(-0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 0.0f}}}},
		{Faces::RIGHT,
		 std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 1.0f}},
							   Vertex{Vector3Float(0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 1.0f}},
							   Vertex{Vector3Float(0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 0.0f}},
							   Vertex{Vector3Float(0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 0.0f}}}},
		{Faces::FRONT,
		 std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 1.0f}},
							   Vertex{Vector3Float(-0.5f, -0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 1.0f}},
							   Vertex{Vector3Float(-0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 0.0f}},
							   Vertex{Vector3Float(0.5f, 0.5f, 0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 0.0f}}}},
		{Faces::BACK,
		 std::array<Vertex, 4>{Vertex{Vector3Float(0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 1.0f}},
							   Vertex{Vector3Float(-0.5f, -0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 1.0f}},
							   Vertex{Vector3Float(-0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {1.0f, 0.0f}},
							   Vertex{Vector3Float(0.5f, 0.5f, -0.5f), Vector3Float(1.0, 1.0, 1.0), {0.0f, 0.0f}}}},
		};
	} // namespace Vox::Game::World::Chuncks

#endif // __FACESDATA_HPP__