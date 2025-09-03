#ifndef __WORLDMANAGER_HPP__
#define __WORLDMANAGER_HPP__

#include "Game/Scenes/World/Utils/Defines.hpp"
#include "Game/Scenes/World/Utils/Vulkan.hpp"

#include <unordered_map>
#include <pstl/parallel_backend_utils.h>
#include <vulkan/vulkan.h>

#include "Game/Scenes/World/Chuncks/VoxelChunck.hpp"

#include "Game/Scenes/Menu/SavesData.hpp"

#include "Game/GameManager.hpp"
#include "Game/Scenes/World/Sc_World.hpp"
#include "Game/Scenes/World/Player/Camera.hpp"
#include "Front/Scenes/ScenesManager.hpp"

#include "Utils/AUpdatable.hpp"

namespace Vox::Game::Scenes::World
{
	class Sc_World;
}

namespace Vox::Game::World
{
	class WorldManager : public virtual Vox::Utils::AUpdatable
	{
	public:
		WorldManager() = delete;

		WorldManager(const Scenes::Menu::Saves::WorldData &wd);

		~WorldManager();

		void InitWorld();


		static WorldManager &GetInstance();

		static Scenes::World::Player::Camera &GetCamera();

		void UpdateBuffer(const size_t &index, const Chuncks::VoxelChunck::ChunckUniform &uniform);

		void Render();

	private:
		std::bitset<Utils::Defines::CHUNCK_AMOUNT> _avalaibleBuffers;

		void UpdateGeneration();

		Utils::Defines::ChunckCoord _playerPreviousChunck;

		std::unordered_map<const Utils::Defines::ChunckCoord, Chuncks::VoxelChunck *, MGL::Vectors::Vector2Hash<int> > _chuncks;
		Scenes::World::Player::Camera _camera;
		Utils::Defines::dbuffer _chunckBuffer;

		/* private */
	};
} // namespace Vox::Game::World
#endif // __WORLDMANAGER_HPP__
