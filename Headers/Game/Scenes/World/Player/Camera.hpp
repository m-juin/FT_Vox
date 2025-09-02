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
			Camera() = delete;
			Camera(Vector3 defaultPos = {0.0, 0.0, 0.0}, Vector3 defaultRot = {0.0f, 0.0f, 0.0f});
			~Camera() {};

		private:
			CameraInfo _info;
			void RebuildInfo();
			bool _isDirty;
			Vector3 _position;
			Vector3 _rotation;

			float _yaw = 90.0;
			float _pitch = 180.0;
			// float _sensitivity = 0.05f;

			Vector3 _front;
			Vector3 _up = Vector3(0.0f, 1.0f, 0.0f);
			Vector3 _rightDir;
			Vector3 _worldUp = Vector3(0.0f, 1.0, 0.0f);
			Vector3 _target = Vector3(0.0f, 0.0f, -1.0f);

			void UpdateVectors();
			/* private */
	};

} // namespace Vox::Game::Scenes::World::Player

#endif // __CAMERA_HPP__