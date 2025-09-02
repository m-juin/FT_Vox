#include "Game/Scenes/World/WorldManager.hpp"

#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

namespace Vox::Game::World
{
	WorldManager::WorldManager(const Scenes::Menu::Saves::WorldData &wd)
		: _camera({0.0, 0.0, -100.0}, {0.0, 0.0, 0.0}), _chunckBuffer(2, Utils::Vulkan::GetAlignedChunckSize() * Utils::Defines::CHUNCK_AMOUNT,
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
	{
		(void)wd;
		this->_firstChunck = new Chuncks::VoxelChunck(0);
		this->_chunckBuffer.Create(nullptr);
		auto pipeline = Front::Rendering::Pipelines::PipelinesManager::GetInstance().operator[]<Front::Rendering::Pipelines::VoxelPipeline>("Voxel");
		pipeline->InitSet({this->_chunckBuffer.GetBuffer(0), this->_chunckBuffer.GetBuffer(1)}, sizeof(Chuncks::VoxelChunck::ChunckUniform));
	}

	WorldManager::~WorldManager()
	{
		if (this->_firstChunck)
			delete this->_firstChunck;
	}

	WorldManager &WorldManager::GetInstance()
	{
		if (auto sc = dynamic_cast<Vox::Game::Scenes::World::Sc_World *>(
				&GameManager::GetInstance().GetSceneManager().GetCurrentScene()))
			return sc->GetWorldManager();
		throw std::runtime_error("Try to get WorldManager when not avalaible!");
	}

    Scenes::World::Player::Camera& WorldManager::GetCamera()
    {
        return WorldManager::GetInstance()._camera;
    }

	void WorldManager::UpdateBuffer(const size_t &index, const Chuncks::VoxelChunck::ChunckUniform &uniform)
	{
		auto frame = Front::Rendering::SyncObjects().GetInstance().GetNextFrame();
		this->_chunckBuffer.UpdateAtOffset(frame, index * Utils::Vulkan::GetAlignedChunckSize(), (void *)&uniform,
										   sizeof(uniform.model));
	}

	void WorldManager::Render()
	{
		this->_firstChunck->Update();
		this->_camera.Update();
		Front::Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("Voxel");
		this->_firstChunck->Render();
	}
} // namespace Vox::Game::World