#include "Game/Models/DynamicObject.hpp"

#include "MathGraphicalLib/Matrix/Operations.hpp"

namespace Vox::Game::Models
{
	DynamicObject::DynamicObject(Vector3 defaultPos, Vector3 defaultRot, uint32_t updateDelay) : AUpdatable(updateDelay)
	{
		this->_position = defaultPos;
		this->_rotation = defaultRot;
		this->_isDirty = true;

		this->onUpdate.AddCallBack([this]() { this->AssignModel(); });
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

			rot1 = MGL::Matrix::Operations::Rotate(rot1, this->_rotation[0], Vector3(1.0f, 0.0f, 0.0f));
			rot2 =  MGL::Matrix::Operations::Rotate(rot2, this->_rotation[1], Vector3(0.0f, 1.0f, 0.0f));
			rot3 =  MGL::Matrix::Operations::Rotate(rot3, this->_rotation[2], Vector3(0.0f, 0.0f, 1.0f));

            rot = rot1 * rot2 * rot3;
		}

        Matrix pos;
        
        { // position
            pos.SetIdentity();
            MGL::Matrix::Operations::Translate(pos, this->_position);
        }

		return Matrix(pos * rot);
	}

	void DynamicObject::SetPos(Vector3 newPos)
	{
		if (newPos == this->_position)
			return;
		this->_isDirty = true;
	}

	void DynamicObject::SetRot(Vector3 newRot)
	{
		if (newRot == this->_rotation)
			return;
		this->_rotation = newRot;
		this->_isDirty = true;
	}

	void DynamicObject::Move(Vector3 moveValue)
	{
		this->_position += moveValue;
		this->_isDirty = true;
	}

	void DynamicObject::Rotate(Vector3 rotValue)
	{
		this->_rotation += rotValue;
		this->_isDirty = true;
	}
} // namespace Vox::Game::Models