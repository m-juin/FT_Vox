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
		this->_bManager = std::make_unique<Generation::BufferManager>();
		this->_gManager = std::make_unique<Generation::GenerationManager>();

		(void)wd;
		this->_chunckBuffer.Create(nullptr);
		auto pipeline = Front::Rendering::Pipelines::PipelinesManager::GetInstance()
							.operator[]<Front::Rendering::Pipelines::VoxelPipeline>("Voxel");
		pipeline->InitSet({this->_chunckBuffer.GetBuffer(0), this->_chunckBuffer.GetBuffer(1)},
						  sizeof(Chuncks::VoxelChunck::ChunckUniform));

		this->InitWorld();
		this->onUpdate.AddCallBack(
			[this]()
			{
				this->UpdateGeneration();
				this->_gManager->Update();
			});
	}

	WorldManager::~WorldManager()
	{
		for (auto pair : this->_chuncks)
			if (pair.second)
				delete pair.second;
	}

	void WorldManager::InitWorld()
	{
		this->_chuncks.reserve(Utils::Defines::CHUNCK_AMOUNT);
		_playerPreviousChunck = {0, 0};
		for (int x = -Utils::Defines::HALF_RENDER_DISTANCE; x < Utils::Defines::HALF_RENDER_DISTANCE; x++)
		{
			for (int y = -Utils::Defines::HALF_RENDER_DISTANCE; y < Utils::Defines::HALF_RENDER_DISTANCE; y++)
			{
				uint32_t buffer = this->_bManager->ReserveBuffer();
				if (buffer >= Utils::Defines::CHUNCK_AMOUNT)
					return;
				auto ch = new Chuncks::VoxelChunck(buffer, {x, 0, y});
				// this->_chuncks[{x, y}] = ch;
				this->_gManager->RequestChuncksGeneration(ch);
				// tManager.EnQueue([ch]() { ch->BuildVoxelObject(); });
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
		this->_chunckBuffer.UpdateAtOffset(frame, index * Utils::Vulkan::GetAlignedChunckSize(), (void *)&uniform,
										   sizeof(Chuncks::VoxelChunck::ChunckUniform));
	}

	void WorldManager::Render()
	{
		for (auto &_pair : this->_chuncks)
			if (_pair.second)
				_pair.second->Update();
		this->_camera.Update();
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Voxel");
		for (auto &_pair : this->_chuncks)
			if (_pair.second)
				_pair.second->Render();
	}

	void WorldManager::UpdateGeneration()
	{
		auto playerPos = _camera.GetPosition();
		const Utils::Defines::ChunckCoord playerCoord = {static_cast<int>(playerPos[0] / Utils::Defines::CHUNCK_SIZE),
														 static_cast<int>(playerPos[2] / Utils::Defines::CHUNCK_SIZE)};

		if (playerCoord == _playerPreviousChunck)
			return;
		_playerPreviousChunck = playerCoord;
		std::vector<Utils::Defines::ChunckCoord> toDelete;
		for (auto _pair : this->_chuncks)
		{
			if ((size_t)std::abs(MGL::Vectors::Dist(_pair.first, playerCoord)) >=
				Utils::Defines::SQUARE_RENDER_DISTANCE)
				toDelete.push_back(_pair.first);
		}
		for (auto pos : toDelete)
		{
			const auto it = this->_chuncks.find(pos);
			const uint16_t index = it->second->GetBuffer();
			this->_chuncks.erase(it);
			this->_bManager->ReleaseBuffer(index);
			delete it->second;
		}
		Utils::Defines::ChunckCoord effectiveCoord;
		for (int x = -Utils::Defines::HALF_RENDER_DISTANCE; x < Utils::Defines::HALF_RENDER_DISTANCE; x++)
		{
			effectiveCoord[0] = playerCoord[0] + x;
			for (int y = -Utils::Defines::HALF_RENDER_DISTANCE; y < Utils::Defines::HALF_RENDER_DISTANCE; y++)
			{
				effectiveCoord[1] = playerCoord[1] + y;
				if ((size_t)std::abs(MGL::Vectors::Dist(effectiveCoord, playerCoord)) >=
					Utils::Defines::SQUARE_RENDER_DISTANCE)
					continue;
				if (this->_chuncks.find(effectiveCoord) == this->_chuncks.end())
				{
					uint16_t buffer = this->_bManager->ReserveBuffer();
					if (buffer > Utils::Defines::CHUNCK_AMOUNT)
						return;

					auto ch = new Chuncks::VoxelChunck(buffer, {effectiveCoord[0], 0, effectiveCoord[1]});
					this->_gManager->RequestChuncksGeneration(ch);
					// this->_chuncks[effectiveCoord] = ch;
					// tManager.EnQueue([ch]() { ch->BuildVoxelObject(); });
				}
			}
		}
	}

	void WorldManager::AddEndedChunck(std::list<Chuncks::VoxelChunck *> chuncks)
	{
		for (auto ch : chuncks)
		{
			auto chPos = ch->GetChunckPosition();
			// std::cout << "Adding Chunck " << chPos << std::endl;
			this->_chuncks[{chPos[0], chPos[2]}] = ch;
		}
	}
} // namespace Vox::Game::World
