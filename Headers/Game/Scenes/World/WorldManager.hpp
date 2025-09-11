#ifndef __WORLDMANAGER_HPP__
#define __WORLDMANAGER_HPP__

#include "Game/Scenes/World/Utils/Defines.hpp"
#include "Game/Scenes/World/Utils/Vulkan.hpp"

#include <list>
#include <unordered_map>
#include <vulkan/vulkan.h>

#include "Game/Scenes/World/Chuncks/ChunckCluster.hpp"

#include "Game/Scenes/Menu/SavesData.hpp"

#include "Front/Scenes/ScenesManager.hpp"
#include "Game/GameManager.hpp"
#include "Game/Scenes/World/Player/Camera.hpp"
#include "Game/Scenes/World/Sc_World.hpp"

#include "Utils/AUpdatable.hpp"

#include "./Generation/BufferManager.hpp"
#include "./Generation/GenerationManager.hpp"

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
			void AddEndedChunck(std::shared_ptr<Chuncks::ChunckCluster> chunck);

			size_t GetLoadedChunckData()
			{
				return this->_chuncks.size();
			};
			const Generation::GenerationManager GetGenerationManager()
			{
				return *this->_gManager;
			};
			Generation::BufferManager &GetBufferManager()
			{
				return *this->_bManager;
			};

			void UpdateSeed(uint64_t newSeed)
			{
				this->_gManager->UpdateSeed(newSeed);
			};

		private:
			void CheckCreation();
			void CheckDeletion();
			void UpdatePlayerPos();

			std::unique_ptr<Generation::BufferManager> _bManager;
			std::unique_ptr<Generation::GenerationManager> _gManager;

			Utils::Defines::ChunckCoord _playerChunck;

			std::unordered_map<const Utils::Defines::ChunckCoord, std::shared_ptr<Chuncks::ChunckCluster>,
							   MGL::Vectors::Vector2Hash<int>>
				_chuncks;
			Scenes::World::Player::Camera _camera;
			Utils::Defines::dbuffer _chunckBuffer;

			/* private */
	};
} // namespace Vox::Game::World
#endif // __WORLDMANAGER_HPP__
