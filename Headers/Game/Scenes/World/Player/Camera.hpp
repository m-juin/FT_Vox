#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "MathGraphicalLib/Matrix/Matrix4.hpp"

#include "Utils/AUpdatable.hpp"

#include "../Utils/Defines.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"
#include "MathGraphicalLib/Utils.hpp"

#include "Front/Rendering/SyncObjects.hpp"
#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"

namespace Vox::Game::Scenes::World::Player
{
	struct CameraInfo
	{
			MGL::Matrix::Matrix4 projection;
			MGL::Matrix::Matrix4 view;
	};

	using namespace Game::Utils::Defines;
	class Camera : public Vox::Utils::AUpdatable
	{
		public:
			void PushConstant(int target);
			Camera() = delete;
			Camera(Vector3Float defaultPos = {0.0, 0.0, 0.0}, Vector3Float defaultRot = {0.0f, 0.0f, 0.0f});
			~Camera() {};

			void HandleMouseMovement(const double &xOffSet, const double &yOffSet);
			void Move(const Vector3Float &axis);

			Vector3Float GetPosition() const;

		private:
			void Rotate(const double &xOff, const double &yOff);

			CameraInfo _worldInfo;
			CameraInfo _skyInfo;
			void RebuildInfo();
			bool _isDirty;
			Vector3Float _position;
			Vector3Float _rotation;

			float _cameraSpeed = 0.5f;

			float _yaw = -90.0;
			float _pitch = 0.0;
			float _sensitivity = 0.05f;

			Vector3Float _front;
			Vector3Float _up = Vector3Float(0.0f, 1.0f, 0.0f);
			Vector3Float _rightDir;
			Vector3Float _worldUp = Vector3Float(0.0f, 1.0, 0.0f);
			Vector3Float _target = Vector3Float(0.0f, 0.0f, -1.0f);

			void UpdateVectors();
			/* private */
	};
	

} // namespace Vox::Game::Scenes::World::Player

#endif // __CAMERA_HPP__