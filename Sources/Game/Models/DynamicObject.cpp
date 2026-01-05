#include "Game/Models/DynamicObject.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"

namespace Vox::Game::Models
{
	DynamicObject::DynamicObject(Vector3float defaultPos, Vector3float defaultRot, uint32_t updateDelay) : AUpdatable(updateDelay)
	{
		this->_position = defaultPos;
		this->_rotation = defaultRot;

		this->_isDirty.fill(true);
		this->onUpdate.AddCallBack(
			[this]()
			{
				auto frame = Front::Rendering::SyncObjects::GetInstance().GetNextFrame();
				if (this->_isDirty[frame] == false)
					return;
				bool assign = this->AssignModel();
				if (assign == false)
					return ;
				this->_isDirty[frame] = false;
			});
	}

	DynamicObject::~DynamicObject() {}

	Matrix DynamicObject::GetModel()
	{

		Matrix rot;

		{ // rotation
			Matrix rot1;
			rot1.SetIdentity();
			Matrix rot2;
			rot2.SetIdentity();
			Matrix rot3;
			rot3.SetIdentity();

			rot1 = MGL::Matrix::Operations::Rotate(rot1, this->_rotation[0], Vector3float(1.0f, 0.0f, 0.0f));
			rot2 = MGL::Matrix::Operations::Rotate(rot2, this->_rotation[1], Vector3float(0.0f, 1.0f, 0.0f));
			rot3 = MGL::Matrix::Operations::Rotate(rot3, this->_rotation[2], Vector3float(0.0f, 0.0f, 1.0f));

			rot = rot1 * rot2 * rot3;
		}

		Matrix pos;

		{ // position
			pos.SetIdentity();
			pos = MGL::Matrix::Operations::Translate(pos, this->_position);
		}

		return Matrix(pos * rot);
	}

	void DynamicObject::SetPos(Vector3float newPos)
	{
		if (newPos == this->_position)
			return;
		this->_position = newPos;

		this->_isDirty.fill(true);
	}

	void DynamicObject::SetRot(Vector3float newRot)
	{
		if (newRot == this->_rotation)
			return;
		this->_rotation = newRot;
		this->_isDirty.fill(true);
	}

	void DynamicObject::Move(Vector3float moveValue)
	{
		this->_position += moveValue;
		this->_isDirty.fill(true);
	}

	void DynamicObject::Rotate(Vector3float rotValue)
	{
		this->_rotation += rotValue;
		this->_isDirty.fill(true);
	}
} // namespace Vox::Game::Models