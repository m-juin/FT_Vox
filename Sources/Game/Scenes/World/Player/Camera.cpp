#include "Game/Scenes/World/Player/Camera.hpp"

namespace Vox::Game::Scenes::World::Player
{
	Camera::Camera(Vector3 defaultPos, Vector3 defaultRot) : Vox::Utils::AUpdatable(1)
	{
		this->_position = defaultPos;
		this->_rotation = defaultRot;

		this->UpdateVectors();
		this->onUpdate.AddCallBack(
			[this]()
			{
				if (this->_isDirty == true)
					this->RebuildInfo();
				using namespace Front::Rendering;
				auto frame = SyncObjects::GetInstance().GetCurrentFrame();

				auto buffer = CommandsPool::GetInstance().GetBuffer(frame);
				auto pipeline =
					Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::VoxelPipeline>("Voxel");
				if (pipeline == nullptr)
					return;

				vkCmdPushConstants(buffer, pipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(this->_info),
								   &this->_info);
			});
	}

	void Camera::RebuildInfo()
	{
		float aspect = 1920.0f / 1080.0f; // ✅ ratio en float
		this->_info.projection =
			MGL::Matrix::Operations::Perspective(MGL::Utils::Radians(45.0f), aspect, 0.001f, 1000.0f);

		this->_info.view = MGL::Matrix::Operations::LookAt(this->_position, this->_position + this->_front, this->_up);

		this->_isDirty = false;
	}

	void Camera::UpdateVectors()
	{
		using namespace MGL::Vectors::Operations;

		Vector3 front;
		// ✅ Mapping clair : [0] = x, [1] = y, [2] = z
		front[0] = cos(MGL::Utils::Radians(this->_yaw)) * cos(MGL::Utils::Radians(this->_pitch)); // X
		front[1] = sin(MGL::Utils::Radians(this->_pitch));										  // Y
		front[2] = sin(MGL::Utils::Radians(this->_yaw)) * cos(MGL::Utils::Radians(this->_pitch)); // Z

		this->_front = Normalize(front);

		this->_rightDir = Normalize(Cross(this->_front, this->_worldUp));
		this->_up = Normalize(Cross(this->_rightDir, this->_front));
        this->_isDirty = true;
	}

} // namespace Vox::Game::Scenes::World::Player
