
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
		float aspect = 1920.0f / 1080.0f;
		this->_info.projection =
			MGL::Matrix::Operations::Perspective(MGL::Utils::Radians(45.0f), aspect, 0.001f, 1000.0f);

		this->_info.view = MGL::Matrix::Operations::LookAt(this->_position, this->_position + this->_front, this->_up);

		this->_isDirty = false;
	}

	void Camera::Rotate(const double &xOff, const double &yOff)
	{
		this->_yaw += xOff * this->_sensitivity;
		this->_pitch += yOff * this->_sensitivity;

		if (this->_pitch > 89.0f)
			this->_pitch = 89.0f;
		if (this->_pitch < -89.0f)
			this->_pitch = -89.0f;

		if (this->_yaw >= 360.0f)
			this->_yaw -= 360.0f;
		if (this->_yaw < 0.0f)
			this->_yaw += 360.0f;

		UpdateVectors();
	}

	void Camera::UpdateVectors()
	{
		using namespace MGL::Vectors::Operations;

		Vector3 front;

		front[0] = cos(MGL::Utils::Radians(this->_yaw)) * cos(MGL::Utils::Radians(this->_pitch)); // X
		front[1] = sin(MGL::Utils::Radians(this->_pitch));										  // Y
		front[2] = sin(MGL::Utils::Radians(this->_yaw)) * cos(MGL::Utils::Radians(this->_pitch)); // Z

		this->_front = Normalize(front);

		this->_rightDir = Normalize(Cross(this->_front, this->_worldUp));
		this->_up = Normalize(Cross(this->_rightDir, this->_front));
		this->_isDirty = true;
	}

	void Camera::HandleMouseMovement(const double &xOffSet, const double &yOffSet)
	{
		// std::cout << "[DEBUG] " << "Mouse xOff = " << xOffSet << " | yOff = " << yOffSet << std::endl;
		this->Rotate(xOffSet, yOffSet);
	}

	void Camera::Move(const Vector3 &axis)
	{
		// std::cout << "[DEBUG] " << "Camera shall move on axis = " << axis << std::endl;
		if (axis[0] != 0)
			this->_position += this->_rightDir * _cameraSpeed * axis[0];
		else if (axis[1] != 0)
			this->_position += this->_worldUp * _cameraSpeed * axis[1];
		else if (axis[2] != 0)
			this->_position += this->_front * _cameraSpeed * axis[2];
		else
			return ;
		this->UpdateVectors();
	}

} // namespace Vox::Game::Scenes::World::Player
