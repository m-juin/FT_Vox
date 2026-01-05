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
#include "Game/Scenes/World/Skybox/SkyBox.hpp"

#include "Utils/AUpdatable.hpp"
#include "Utils/ALateUpdatable.hpp"

#include "./Generation/BufferManager.hpp"
#include "./Generation/GenerationManager.hpp"

#include "MathGraphicalLib/Vectors/Defines.hpp"

namespace Vox::Game::Scenes::World
{
	class Sc_World;
}

namespace Vox::Game::World
{
	using namespace MGL::Vectors::Types;
	class WorldManager : public virtual Vox::Utils::AUpdatable, public virtual Vox::Utils::ALateUpdatable
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

			std::shared_ptr<Chuncks::ChunckCluster> GetCluster(Vector2Int coord);

			size_t GetLoadedChunckData()
			{
				return this->_chuncks.size();
			};
			const Generation::GenerationManager &GetGenerationManager() const
			{
				return *this->_gManager;
			};
			Generation::GenerationManager &GetGenerationManager()
			{
				return *this->_gManager;
			};
			Generation::BufferManager &GetBufferManager()
			{
				return *this->_bManager;
			};

			void RequestChunckRefresh();

			void UpdateSeed(uint64_t newSeed)
			{
				if (this->_gManager->UpdateSeed(newSeed) == false)
					return;

				this->RequestChunckRefresh();
			};

			const Vector2Int & GetPlayerChunck() {return this->_playerChunck;};

			void ReleaseChunkBuffer(const size_t &bufferIndex);
			size_t RequestChunkBuffer();

		private:
			void CheckCreation();
			void CheckDeletion();
			void UpdatePlayerPos();

			std::unique_ptr<Generation::BufferManager> _bManager;
			std::unique_ptr<Generation::GenerationManager> _gManager;

			Vector2Int _playerChunck = {0, 0};

			std::unordered_map<const Vector2Int, std::shared_ptr<Chuncks::ChunckCluster>,
							   MGL::Vectors::Vector2Hash<int>>
				_chuncks;
			Scenes::World::Player::Camera _camera;
			Utils::Defines::dbuffer _chunckBuffer;
			std::unique_ptr<Game::World::Skybox::SkyBox> _skyBox;

			/* private */
	};
} // namespace Vox::Game::World
#endif // __WORLDMANAGER_HPP__
