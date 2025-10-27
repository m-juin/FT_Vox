#include "Game/Scenes/World/WorldManager.hpp"

#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/TransparentVoxelPipeline.hpp"
#include "Front/Rendering/Pipelines/SkyBoxPipeline.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include <cstring>

#include "Game/ThreadManager.hpp"

namespace Vox::Game::World
{
	WorldManager::WorldManager(const Scenes::Menu::Saves::WorldData &wd)
		: _camera({0.0, 160.0, 0.0}, {0.0, 0.0, 0.0}),
		  _chunckBuffer(2, Utils::Vulkan::GetAlignedChunckSize() * Utils::Defines::CHUNCK_AMOUNT,
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
	{
		this->_gManager = std::make_unique<Generation::GenerationManager>(std::stoull(wd.seed.c_str()));
		this->_bManager = std::make_unique<Generation::BufferManager>();

		this->_chunckBuffer.Create(nullptr);
		auto pipeline = Front::Rendering::Pipelines::PipelinesManager::GetInstance()
							.operator[]<Front::Rendering::Pipelines::VoxelPipeline>("Voxel");
		pipeline->InitSet({this->_chunckBuffer.GetBuffer(0), this->_chunckBuffer.GetBuffer(1)},
						  sizeof(Chuncks::VoxelChunck::ChunckUniform));

		auto pipeline2 = Front::Rendering::Pipelines::PipelinesManager::GetInstance()
							 .operator[]<Front::Rendering::Pipelines::TransparentVoxelPipeline>("Voxel_Transparent");
		pipeline2->InitSet({this->_chunckBuffer.GetBuffer(0), this->_chunckBuffer.GetBuffer(1)},
						   sizeof(Chuncks::VoxelChunck::ChunckUniform));

		auto pipeline3 = Front::Rendering::Pipelines::PipelinesManager::GetInstance()
							 .operator[]<Front::Rendering::Pipelines::SkyBoxPipeline>("SkyBox");
		pipeline3->InitSet();
		this->InitWorld();
		this->onUpdate.AddCallBack(
			[this]()
			{
				this->UpdatePlayerPos();
				this->_gManager->SetPlayerPos(this->_playerChunck);
				this->CheckDeletion();
				this->CheckCreation();
				this->_gManager->Update();
				for (auto &_pair : this->_chuncks)
					if (_pair.second)
						_pair.second->Update();
			});
	}

	WorldManager::~WorldManager() {}

	void WorldManager::InitWorld()
	{
		this->_skyBox = std::make_unique<Game::World::Skybox::SkyBox>();
		this->_chuncks.reserve(Utils::Defines::CHUNCK_BUFFER_AMOUNT);
		_playerChunck = {0, 0};
		for (int x = -Utils::Defines::RENDER_DISTANCE; x < Utils::Defines::RENDER_DISTANCE; x++)
		{
			for (int y = -Utils::Defines::RENDER_DISTANCE; y < Utils::Defines::RENDER_DISTANCE; y++)
			{
				Utils::Defines::ChunckCoord coord = {x, y};
				if (this->_gManager->IsChunckPresent(coord) == true || _chuncks.find(coord) != _chuncks.end())
				{
					continue;
				}
				this->_gManager->RequestChuncksGeneration(coord);
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
		auto frame = Vox::Front::Rendering::SyncObjects::GetInstance().GetNextFrame();
		this->_chunckBuffer.UpdateAtOffset(frame, index * Utils::Vulkan::GetAlignedChunckSize(), (void *)&uniform,
										   sizeof(Chuncks::VoxelChunck::ChunckUniform));
	}

	void WorldManager::Render()
	{
		this->_camera.Update();
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Skybox");
		this->_skyBox->Render();
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Voxel");
		for (auto &_pair : this->_chuncks)
			if (_pair.second)
				_pair.second->Render(0);
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Voxel_Transparent");
		for (auto &_pair : this->_chuncks)
			if (_pair.second)
				_pair.second->Render(1);
	}

	void WorldManager::CheckCreation()
	{
		Utils::Defines::ChunckCoord effectiveCoord;
		for (int x = -Utils::Defines::RENDER_DISTANCE; x < Utils::Defines::RENDER_DISTANCE; x++)
		{
			effectiveCoord[0] = _playerChunck[0] + x;
			for (int y = -Utils::Defines::RENDER_DISTANCE; y < Utils::Defines::RENDER_DISTANCE; y++)
			{
				effectiveCoord[1] = _playerChunck[1] + y;

				if (MGL::Vectors::Dist(effectiveCoord, _playerChunck) >= Utils::Defines::SQUARE_RENDER_DISTANCE)
					continue;

				if (_chuncks.find(effectiveCoord) != _chuncks.end() || _gManager->IsChunckPresent(effectiveCoord))
					continue;

				_gManager->RequestChuncksGeneration(effectiveCoord);
			}
		}
	}

	void WorldManager::RequestChunckRefresh()
	{
		for (auto cluster : this->_chuncks)
		{
			// const auto it = this->_chuncks.find(pos);
			const uint16_t index = cluster.second->GetBuffer();
			// this->_chuncks.erase(it);
			this->_bManager->ReleaseBuffer(index);
		}
		this->_chuncks.clear();
	}

	void WorldManager::CheckDeletion()
	{
		std::vector<Utils::Defines::ChunckCoord> toDelete;
		for (auto _pair : this->_chuncks)
		{
			// std::cout << _pair.first << std::endl;
			if (MGL::Vectors::Dist(_pair.first, _playerChunck) >= Utils::Defines::SQUARE_RENDER_DISTANCE)
				toDelete.push_back(_pair.first);
		}
		// std::cout << std::endl;
		for (auto pos : toDelete)
		{
			const auto it = this->_chuncks.find(pos);
			const uint16_t index = it->second->GetBuffer();
			this->_chuncks.erase(it);
			this->_bManager->ReleaseBuffer(index);
		}
	}

	void WorldManager::UpdatePlayerPos()
	{
		auto playerPos = _camera.GetPosition();
		const Utils::Defines::ChunckCoord playerCoord =
			Utils::Defines::ChunckCoord(std::floor(playerPos[0] / Utils::Defines::CHUNCK_SIZE),
										std::floor(playerPos[2] / Utils::Defines::CHUNCK_SIZE));

		if (playerCoord == _playerChunck)
			return;
		_playerChunck = playerCoord;
	}

	void WorldManager::AddEndedChunck(std::shared_ptr<Chuncks::ChunckCluster> chunck)
	{
		if (chunck)
			this->_chuncks[chunck->GetPosition()] = chunck;
	}

} // namespace Vox::Game::World
