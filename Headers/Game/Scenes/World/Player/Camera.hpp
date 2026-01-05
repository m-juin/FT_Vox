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

#include "Front/Rendering/Frustum/Frustum.hpp"

#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"

#include "MathGraphicalLib/Vectors/Defines.hpp"

namespace Vox::Game::Scenes::World::Player
{
	struct CameraInfo
	{
			MGL::Matrix::Matrix4 projection;
			MGL::Matrix::Matrix4 view;
	};

	using namespace Game::Utils::Defines;
	using namespace MGL::Vectors::Types;
	class Camera : public Vox::Utils::AUpdatable
	{
		public:
			void PushConstant(int target);
			Camera() = delete;
			Camera(Vector3float defaultPos = {0.0, 0.0, 0.0}, Vector3float defaultRot = {0.0f, 0.0f, 0.0f});
			~Camera() {};

			void HandleMouseMovement(const double &xOffSet, const double &yOffSet);
			void Move(const Vector3float &axis);

			Vector3float GetPosition() const;

			const Front::Rendering::Frustum::Frustum &GetFrustum() {return this->_viewingfrustum;}

		private:

			void CreateFrustum();
			void RebuildInfo();
			void UpdateVectors();


			void Rotate(const double &xOff, const double &yOff);
			MGL::Matrix::Matrix4 SkyboxView(MGL::Matrix::Matrix4 &cameraView);

			CameraInfo _worldInfo;
			CameraInfo _skyInfo;
			bool _isDirty;
			Vector3float _position;
			Vector3float _rotation;

			float _cameraSpeed = 0.5f;

			float _yaw = -90.0;
			float _pitch = 0.0;
			float _sensitivity = 0.05f;

			Vector3float _front;
			Vector3float _up = Vector3float(0.0f, 1.0f, 0.0f);
			Vector3float _rightDir;
			Vector3float _worldUp = Vector3float(0.0f, 1.0, 0.0f);
			Vector3float _target = Vector3float(0.0f, 0.0f, -1.0f);

			Front::Rendering::Frustum::Frustum _viewingfrustum;

			const float _fov = MGL::Utils::Radians(45.0f);
			float _aspect = 1920.0f / 1080.0f;
			float _near = 0.1f;
			float _far = 1000.0f;

	};
	

} // namespace Vox::Game::Scenes::World::Player

#endif // __CAMERA_HPP__