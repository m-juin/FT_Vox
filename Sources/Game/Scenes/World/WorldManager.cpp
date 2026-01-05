#include "Game/Scenes/World/WorldManager.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/SkyBoxPipeline.hpp"
#include "Front/Rendering/Pipelines/TransparentVoxelPipeline.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"
#include <cstring>

#include "Game/ThreadManager.hpp"

#include "LoggerLib/UtilityFunctions.hpp"

#include "Utils/TracyUtils.hpp"

#include "Game/Scenes/World/Generation/BuffersCleanupManager.hpp"

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
							 .operator[]<Front::Rendering::Pipelines::SkyBoxPipeline>("Skybox");
		pipeline3->InitSet();
		this->InitWorld();
		this->onUpdate.AddCallBack(
			[this]()
			{
				this->UpdatePlayerPos();
				this->_gManager->SetPlayerPos(this->_playerChunck);
				this->CheckDeletion();
				this->CheckCreation();
				{
#ifdef TRACY_ENABLE
					ZoneScopedNC("Generation Manager Update", tracy::Color::Green1);
#endif
					this->_gManager->Update();
				}
				{
#ifdef TRACY_ENABLE
					ZoneScopedNC("Chunck cluster Update", tracy::Color::Green1);
#endif
					for (auto &_pair : this->_chuncks)
					{
						if (_pair.second)
						{
#ifdef TRACY_ENABLE
							std::stringstream ss;
							ss << _pair.first;
							ZoneScopedNC("Single Update Cluster", tracy::Color::White);
							ZoneText(ss.str().c_str(), ss.str().size());
#endif
							_pair.second->Update();
						}
					}
				}
			});
	
			this->onLateUpdate.AddCallBack([this](){
				// this->_gManager->GetCleanupManager()->ProcessFrameCleanup();
				this->_gManager->LateUpdate();
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
				Vector2Int coord = {x, y};
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
#ifdef TRACY_ENABLE
		ZoneScoped;
#endif

		auto frame = Vox::Front::Rendering::SyncObjects::GetInstance().GetNextFrame();
		this->_chunckBuffer.UpdateAtOffset(frame, index * Utils::Vulkan::GetAlignedChunckSize(), (void *)&uniform,
										   sizeof(Chuncks::VoxelChunck::ChunckUniform));
	}

	void WorldManager::Render()
	{
		this->_camera.Update();
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Skybox");
		this->_camera.PushConstant(1);
		this->_skyBox->Render();
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Voxel");
		this->_camera.PushConstant(0);
		std::pair<size_t, size_t> counters = {0, 0};
		for (auto &_pair : this->_chuncks)
		{
			if (_pair.second)
			{
				auto pair2 = _pair.second->Render(0);
				counters.first += pair2.first;
				counters.second += pair2.second;
			}
		}
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Voxel_Transparent");
		for (auto &_pair : this->_chuncks)
			if (_pair.second)
				_pair.second->Render(1);

		// // LoggerLib::LogDebug("Try : ", counters.first, " | ", "Effectives : ", counters.second);
	}

	void WorldManager::CheckCreation()
	{
#ifdef TRACY_ENABLE
		ZoneScopedNC("Chekc Chunk Creation Update", tracy::Color::Purple1);
#endif
		Vector2Int effectiveCoord;
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
		// for (auto cluster : this->_chuncks)
		// {
		// const uint16_t index = cluster.second->GetBuffer();
		// this->_bManager->ReleaseBuffer(index);
		// }
		this->_chuncks.clear();
	}

	void WorldManager::ReleaseChunkBuffer(const size_t &bufferIndex)
	{
		if (bufferIndex < Utils::Defines::CHUNCK_AMOUNT)
			this->_bManager->ReleaseBuffer(bufferIndex);
	}

	size_t WorldManager::RequestChunkBuffer()
	{
		return this->_bManager->ReserveBuffer();
	}
	void WorldManager::CheckDeletion()
	{
#ifdef TRACY_ENABLE
		ZoneScopedNC("Check Chunck Deletion", tracy::Color::Aquamarine);
#endif
		std::vector<Vector2Int> toDelete;
		for (auto _pair : this->_chuncks)
		{
			if (MGL::Vectors::Dist(_pair.first, _playerChunck) >= Utils::Defines::SQUARE_RENDER_DISTANCE)
				toDelete.push_back(_pair.first);
		}
		for (auto pos : toDelete)
		{
			const auto it = this->_chuncks.find(pos);
			// const uint16_t index = it->second->GetBuffer();
			// this->_bManager->ReleaseBuffer(index);
			this->_chuncks.erase(it);
		}
	}

	void WorldManager::UpdatePlayerPos()
	{
#ifdef TRACY_ENABLE
		ZoneScopedNC("Update Player Pos Update", tracy::Color::VioletRed);
#endif
		auto playerPos = _camera.GetPosition();
		const Vector2Int playerCoord =
			Vector2Int(std::floor(playerPos[0] / Utils::Defines::CHUNCK_SIZE),
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

	std::shared_ptr<Chuncks::ChunckCluster> WorldManager::GetCluster(Vector2Int coord)
	{
		auto it = this->_chuncks.find(coord);
		if (it == this->_chuncks.end())
			return nullptr;
		return it->second;
	}

} // namespace Vox::Game::World
