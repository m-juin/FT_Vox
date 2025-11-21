
#include "Game/Scenes/World/Player/Camera.hpp"
#include "Front/Rendering/Pipelines/SkyBoxPipeline.hpp"

#include "Front/Rendering/Frustum/BoxCollider.hpp"
#include "Front/Rendering/Frustum/Frustum.hpp"

#include "Front/Rendering/Utils/Vertex/VoxelVertex.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include <cassert>

namespace Vox::Game::Scenes::World::Player
{

	void Camera::PushConstant(int target)
	{
		using namespace Front::Rendering;
		auto frame = SyncObjects::GetInstance().GetCurrentFrame();

		auto buffer = CommandsPool::GetInstance().GetBuffer(frame);
		if (target == 0)
		{
			auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::VoxelPipeline>("Voxel");
			if (pipeline == nullptr)
				return;

			vkCmdPushConstants(buffer, pipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(this->_worldInfo),
							   &this->_worldInfo);
		}

		if (target == 1)
		{

			auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::SkyBoxPipeline>("Skybox");
			vkCmdPushConstants(buffer, pipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(this->_skyInfo),
							   &this->_skyInfo);
		}
	}

	Camera::Camera(Vector3Float defaultPos, Vector3Float defaultRot) : Vox::Utils::AUpdatable(1)
	{
		this->_position = defaultPos;
		this->_rotation = defaultRot;

		this->UpdateVectors();
		this->onUpdate.AddCallBack(
			[this]()
			{
				if (this->_isDirty == true)
					this->RebuildInfo();
			});
	}

	MGL::Matrix::Matrix4 Camera::SkyboxView(MGL::Matrix::Matrix4 &cameraView)
	{
		MGL::Matrix::Matrix4 skyView = cameraView;

		skyView(3, 0) = 0.0f;
		skyView(3, 1) = 0.0f;
		skyView(3, 2) = 0.0f;
		skyView(3, 3) = 1.0f;

		return skyView;
	}

	void Camera::CreateDebugBuffer()
	{
		if (this->_debugVertex.empty())
			return;
		this->_frustumBuffer = std::make_unique<Front::Rendering::Utils::Buffers::StaticBuffer>(
			1, this->_debugVertex.size() * sizeof(Front::Rendering::Utils::Vertex::VoxelVertex),
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		this->_frustumBuffer->Create(this->_debugVertex.data());
	}
	void Camera::RebuildInfo()
	{
		this->_worldInfo.projection =
			MGL::Matrix::Operations::LookAt(this->_position, this->_position + this->_front, this->_up);

		this->_skyInfo.projection = this->_worldInfo.projection;
		this->_worldInfo.view =
			MGL::Matrix::Operations::Perspective(this->_fov, this->_aspect, this->_near, this->_far);
		this->_skyInfo.view = SkyboxView(this->_worldInfo.view);

		this->CreateFrustum();
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

		Vector3Float front;

		front[0] = cos(MGL::Utils::Radians(this->_yaw)) * cos(MGL::Utils::Radians(this->_pitch)); // X
		front[1] = sin(MGL::Utils::Radians(this->_pitch));										  // Y
		front[2] = sin(MGL::Utils::Radians(this->_yaw)) * cos(MGL::Utils::Radians(this->_pitch)); // Z

		this->_front = MGL::Vectors::Operations::Normalize(front);

		this->_rightDir =
			MGL::Vectors::Operations::Normalize(MGL::Vectors::Operations::Cross(this->_front, this->_worldUp));
		this->_up = MGL::Vectors::Operations::Normalize(MGL::Vectors::Operations::Cross(this->_rightDir, this->_front));
		this->_isDirty = true;
	}

	void Camera::RenderFrustum()
	{
		if (this->_shallDrawDebug == false || this->_frustumBuffer == nullptr)
			return;
		using namespace Front::Rendering;
		auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::VoxelPipeline>("Voxel");
		if (pipeline == nullptr)
			return;
		auto frame = SyncObjects::GetInstance().GetCurrentFrame();
		VkDeviceSize offset = {0};
		uint32_t dynamicOffset = 0;
		auto buffer = CommandsPool::GetInstance().GetBuffer(frame);
		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1,
								&pipeline->GetSet(frame), 1, &dynamicOffset);
		vkCmdBindVertexBuffers(buffer, 0, 1, &this->_frustumBuffer->GetBuffer(0), &offset);
		vkCmdDraw(buffer, this->_debugVertex.size(), 1, 0, 0);
	}

	void Camera::HandleMouseMovement(const double &xOffSet, const double &yOffSet)
	{
		this->Rotate(xOffSet, yOffSet);
	}

	void Camera::Move(const Vector3Float &axis)
	{
		if (axis[0] != 0)
			this->_position += this->_rightDir * _cameraSpeed * axis[0];
		else if (axis[1] != 0)
			this->_position += this->_worldUp * _cameraSpeed * axis[1];
		else if (axis[2] != 0)
			this->_position += this->_front * _cameraSpeed * axis[2];
		else
			return;
		this->UpdateVectors();
	}

	Vector3Float Camera::GetPosition() const
	{
		return this->_position;
	}

	void Camera::ChangeDebug()
	{
		if (this->_shallDrawDebug == false)
			this->CreateDebugBuffer();
		this->_shallDrawDebug = !this->_shallDrawDebug;
	}

	void Camera::CreateFrustum()
	{
		using namespace Front::Rendering::Frustum;
		using namespace MGL::Vectors::Operations;

		MGL::Matrix::Matrix4 vp = this->_skyInfo.projection * this->_skyInfo.view;
		this->_frustum = Frustum::ExtractFrustum(vp);

		auto corners = this->_frustum.GetFrustumCorners();
		this->_debugVertex = {// Near plane
							  {corners.ntl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ntr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ntr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.nbr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.nbr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.nbl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.nbl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ntl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},

							  // Far plane
							  {corners.ftl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ftr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ftr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.fbr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.fbr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.fbl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.fbl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ftl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},

							  // Connections
							  {corners.ntl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ftl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ntr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.ftr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.nbl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.fbl, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.nbr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0},
							  {corners.fbr, {1.0, 1.0, 1.0}, {1.0, 1.0}, 0}};
		for (auto &c : _debugVertex)
		{
			MGL::Vectors::Vector4<float> op =  this->_worldInfo.view * MGL::Vectors::Vector4<float>(c.vertPos[0], c.vertPos[1], c.vertPos[2], 1.f);
			c.vertPos = {op[0], op[1], op[2]};
		}
	}
	void Camera::DebugFrustum()
	{
		// // Vérifier les normales
		// for (auto &plane :
		// 	 {&_Frustum.near, &_Frustum.far, &_Frustum.right, &_Frustum.left, &_Frustum.top, &_Frustum.bot})
		// {
		// 	float length = MGL::Vectors::Operations::Length(plane->normal);
		// 	assert(std::abs(length - 1.0f) < 0.001f);
		// }

		// // Vérifier les distances
		// Vector3Float cameraPos = this->_position;
		// assert(_Frustum.near.distance >= 0);
		// assert(_Frustum.far.distance <= 0);
	}
} // namespace Vox::Game::Scenes::World::Player
