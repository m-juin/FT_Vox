
#include "Game/Scenes/World/Player/Camera.hpp"
#include "Front/Rendering/Pipelines/SkyBoxPipeline.hpp"

#include "Front/Rendering/Frustrum/BoxCollider.hpp"
#include "Front/Rendering/Frustrum/Frustrum.hpp"

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

	void Camera::RebuildInfo()
	{
		this->_worldInfo.projection =
			MGL::Matrix::Operations::LookAt(this->_position, this->_position + this->_front, this->_up);

		this->_skyInfo.projection = this->_worldInfo.projection;
		this->_worldInfo.view =
			MGL::Matrix::Operations::Perspective(this->_fov, this->_aspect, this->_near, this->_far);
		this->_skyInfo.view = SkyboxView(this->_worldInfo.view);

		this->CreateFrustrum();
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

		this->_front = Normalize(front);

		this->_rightDir = Normalize(Cross(this->_front, this->_worldUp));
		this->_up = Normalize(Cross(this->_rightDir, this->_front));
		this->_isDirty = true;
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

	void Camera::CreateFrustrum()
	{
		const float halfVSide = this->_far * tanf(this->_fov * .5f);
		const float halfHSide = halfVSide * this->_aspect;
		const MGL::Vectors::Vector3 frontMultFar = this->_front * this->_far;

		// NEAR PLANE - CORRIGÉ
		this->_frustrum.near = {this->_position + this->_front * this->_near, this->_front};

		// FAR PLANE - CORRIGÉ
		this->_frustrum.far = {this->_position + frontMultFar, this->_front * -1.f};

		// PLANS LATÉRAUX - CORRIGÉS (utiliser des points sur les plans, pas la position caméra)
		const MGL::Vectors::Vector3 farCenter = this->_position + frontMultFar;

		// RIGHT PLANE
		// TOP et BOTTOM simplifiés
		this->_frustrum.top = {farCenter + this->_up * halfVSide, this->_up};
		this->_frustrum.bot = {farCenter - this->_up * halfVSide, this->_up * -1.f};

		// Gardez RIGHT/LEFT comme avant
		MGL::Vectors::Vector3 rightNormal =
			MGL::Vectors::Operations::Cross(this->_up, frontMultFar + this->_rightDir * halfHSide);
		rightNormal = MGL::Vectors::Operations::Normalize(rightNormal);
		this->_frustrum.right = {farCenter + this->_rightDir * halfHSide, rightNormal};

		MGL::Vectors::Vector3 leftNormal =
			MGL::Vectors::Operations::Cross(frontMultFar - this->_rightDir * halfHSide, this->_up);
		leftNormal = MGL::Vectors::Operations::Normalize(leftNormal);
		this->_frustrum.left = {farCenter - this->_rightDir * halfHSide, leftNormal};

		// Front::Rendering::Frustrum::Colliders::BoxCollider testAABB(this->_position - Vector3Float(8, 8, 8),
		// 															Vector3Float(16, 16, 16));

		// std::cout << "=== FRUSTUM NORMALS ===" << std::endl;
		// std::cout << "Near: " << this->_frustrum.near.normal << std::endl;
		// std::cout << "Far: " << this->_frustrum.far.normal << std::endl;
		// std::cout << "Right: " << this->_frustrum.right.normal << std::endl;
		// std::cout << "Left: " << this->_frustrum.left.normal << std::endl;
		// std::cout << "Top: " << this->_frustrum.top.normal << std::endl;
		// std::cout << "Bottom: " << this->_frustrum.bot.normal << std::endl;
		// std::cout << "=== TEST AABB AT CAMERA ===" << std::endl;
		// std::cout << "AABB min: " << testAABB.origin << " max: " << testAABB.max << std::endl;

		// // 2. Test plan par plan
		// bool near = testAABB.IsOnForwardPlane(this->_frustrum.near);
		// bool far = testAABB.IsOnForwardPlane(this->_frustrum.far);
		// bool right = testAABB.IsOnForwardPlane(this->_frustrum.right);
		// bool left = testAABB.IsOnForwardPlane(this->_frustrum.left);
		// bool top = testAABB.IsOnForwardPlane(this->_frustrum.top);
		// bool bottom = testAABB.IsOnForwardPlane(this->_frustrum.bot);

		// std::cout << "Plan results:" << std::endl;
		// std::cout << "Near: " << near << " Far: " << far << std::endl;
		// std::cout << "Right: " << right << " Left: " << left << std::endl;
		// std::cout << "Top: " << top << " Bottom: " << bottom << std::endl;
		// std::cout << "ALL: " << (near && far && right && left && top && bottom) << std::endl;
		// std::cout << "=== CAMERA VECTORS DEBUG ===" << std::endl;
		// std::cout << "Front: " << this->_front << std::endl;
		// std::cout << "Right: " << this->_rightDir << std::endl;
		// std::cout << "Up: " << this->_up << std::endl;
		// std::cout << "WorldUp: " << this->_worldUp << std::endl;

		// // Test d'orthogonalité
		// float dotFU = MGL::Vectors::Operations::Dot(this->_front, this->_up);
		// float dotFR = MGL::Vectors::Operations::Dot(this->_front, this->_rightDir);
		// float dotRU = MGL::Vectors::Operations::Dot(this->_rightDir, this->_up);

		// std::cout << "Front·Up: " << dotFU << " (devrait être ~0)" << std::endl;
		// std::cout << "Front·Right: " << dotFR << " (devrait être ~0)" << std::endl;
		// std::cout << "Right·Up: " << dotRU << " (devrait être ~0)" << std::endl;

		// // Test du système de coordonnées
		// MGL::Vectors::Vector3 testCross = MGL::Vectors::Operations::Cross(this->_rightDir, this->_up);
		// std::cout << "Right × Up: " << testCross << " (devrait être ≈ Front)" << std::endl;
	}
} // namespace Vox::Game::Scenes::World::Player
