#ifndef __OPERATION_HPP__
#define __OPERATION_HPP__

#include "./Matrix4.hpp"

#include "../Vectors/Vector3.hpp"

namespace MGL::Matrix::Operations
{
	Matrix4 Rotate(Matrix4 &m1, float angle, Vectors::Vector3<float> axis);
	Matrix4 LookAt(Vectors::Vector3<float> CPos, Vectors::Vector3<float> TPos, Vectors::Vector3<float> DUp);
	Matrix4 Perspective(float fov, float ratio, float far, float near);
	Matrix4 Translate(Matrix4 &m1, Vectors::Vector3<float> dest);
} // namespace MGL::Matrix::Operations

#endif // __OPERATION_HPP__