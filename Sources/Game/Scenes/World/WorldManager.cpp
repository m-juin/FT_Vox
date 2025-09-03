#include "Game/Scenes/World/WorldManager.hpp"

#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

namespace Vox::Game::World
{
	WorldManager::WorldManager(const Scenes::Menu::Saves::WorldData &wd)
		: _camera({0.0, 0.0, -10.0}, {0.0, 0.0, 0.0}),
		  _chunckBuffer(2, Utils::Vulkan::GetAlignedChunckSize() * Utils::Defines::CHUNCK_AMOUNT,
		                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
	{
		(void) wd;
		this->_avalaibleBuffers.flip();
		this->_chunckBuffer.Create(nullptr);
		auto pipeline = Front::Rendering::Pipelines::PipelinesManager::GetInstance()
				.operator[]<Front::Rendering::Pipelines::VoxelPipeline>("Voxel");
		pipeline->InitSet({this->_chunckBuffer.GetBuffer(0), this->_chunckBuffer.GetBuffer(1)},
		                  sizeof(Chuncks::VoxelChunck::ChunckUniform));

		this->InitWorld();
		this->onUpdate.AddCallBack([this]()
		{
			this->UpdateGeneration();
		});
	}

	WorldManager::~WorldManager()
	{
		for (auto pair: this->_chuncks)
			if (pair.second)
				delete pair.second;
	}

	void WorldManager::InitWorld()
	{
		this->_chuncks.reserve(Utils::Defines::CHUNCK_AMOUNT);
		size_t count = 0;
		_playerPreviousChunck = {0, 0};
		for (int x = -Utils::Defines::HALF_RENDER_DISTANCE; x < Utils::Defines::HALF_RENDER_DISTANCE; x++)
		{
			for (int y = -Utils::Defines::HALF_RENDER_DISTANCE; y < Utils::Defines::HALF_RENDER_DISTANCE; y++)
			{
				this->_chuncks[{x, y}] = new Chuncks::VoxelChunck(count, {
					                                                  static_cast<float>(x) *
					                                                  Utils::Defines::CHUNCK_SIZE,
					                                                  0.0,
					                                                  static_cast<float>(y) *
					                                                  Utils::Defines::CHUNCK_SIZE
				                                                  });
				this->_avalaibleBuffers.set(count, false);
				count++;
			}
		}
	}

	WorldManager &WorldManager::GetInstance()
	{
		if (auto sc = dynamic_cast<Scenes::World::Sc_World *>(
			&GameManager::GetInstance().GetSceneManager().GetCurrentScene()))
			return sc->GetWorldManager();
		throw std::runtime_error("Try to get WorldManager when not avalaible!");
	}

	Scenes::World::Player::Camera &WorldManager::GetCamera()
	{
		return GetInstance()._camera;
	}

	void WorldManager::UpdateBuffer(const size_t &index, const Chuncks::VoxelChunck::ChunckUniform &uniform)
	{
		auto frame = Front::Rendering::SyncObjects().GetInstance().GetNextFrame();
		// std::cout << "[DEBUG] " << "uniform of index " << index << " = " << uniform.model << std::endl;
		this->_chunckBuffer.UpdateAtOffset(frame, index * Utils::Vulkan::GetAlignedChunckSize(), (void *) &uniform,
		                                   sizeof(Chuncks::VoxelChunck::ChunckUniform));
	}

	void WorldManager::Render()
	{
		for (auto &_pair: this->_chuncks)
			if (_pair.second)
				_pair.second->Update();
		this->_camera.Update();
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Voxel");
		for (auto &_pair: this->_chuncks)
			if (_pair.second)
				_pair.second->Render();
	}

	void WorldManager::UpdateGeneration()
	{
		auto playerPos = _camera.GetPosition();
		const Utils::Defines::ChunckCoord playerCoord = {
			static_cast<int>(playerPos[0] / Utils::Defines::CHUNCK_SIZE),
			static_cast<int>(playerPos[2] / Utils::Defines::CHUNCK_SIZE)
		};

		if (playerCoord == _playerPreviousChunck)
			return;
		_playerPreviousChunck = playerCoord;
		std::cout << "[DEBUG] " << "New Player Chunck = " << playerCoord << std::endl;
		std::vector<Utils::Defines::ChunckCoord> toDelete;
		for (auto _pair: this->_chuncks)
		{
			if ((size_t)std::abs(MGL::Vectors::Dist(_pair.first, playerCoord)) >= Utils::Defines::SQUARE_RENDER_DISTANCE)
				toDelete.push_back(_pair.first);
		}
		for (auto pos: toDelete)
		{
			const auto it = this->_chuncks.find(pos);
			const uint16_t index = it->second->GetBuffer();
			this->_chuncks.erase(it);
			this->_avalaibleBuffers.set(index);
			std::cout << "[DEBUG] Releasing buffer index: " << index << std::endl;
			delete it->second;
		}

		Utils::Defines::ChunckCoord effectiveCoord;
		for (int x = -Utils::Defines::HALF_RENDER_DISTANCE; x < Utils::Defines::HALF_RENDER_DISTANCE; x++)
		{
			effectiveCoord[0] = playerCoord[0] + x;
			for (int y = -Utils::Defines::HALF_RENDER_DISTANCE; y < Utils::Defines::HALF_RENDER_DISTANCE; y++)
			{
				effectiveCoord[1] = playerCoord[1] + y;
				if ((size_t)std::abs(MGL::Vectors::Dist(effectiveCoord, playerCoord)) >= Utils::Defines::SQUARE_RENDER_DISTANCE)
					continue;
				if (this->_chuncks.find(effectiveCoord) == this->_chuncks.end())
				{
					uint16_t buffer = this->_avalaibleBuffers._Find_first();
					std::cout << "[DEBUG] Next free buffer index: " << buffer << std::endl;
					if (buffer == this->_avalaibleBuffers.size())
					{
						return ;
					}
					this->_avalaibleBuffers.set(buffer, false);

					this->_chuncks[effectiveCoord] = new Chuncks::VoxelChunck(buffer, {
						                                                          static_cast<float>(effectiveCoord[0]) *
							                                                          Utils::Defines::CHUNCK_SIZE,
						                                                          0,
						                                                          static_cast<float>(effectiveCoord[1]) *
						                                                          Utils::Defines::CHUNCK_SIZE

					                                                          });
				}
			}
		}
	}
} // namespace Vox::Game::World
