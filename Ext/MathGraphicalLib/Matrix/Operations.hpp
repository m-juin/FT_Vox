#ifndef __MATRIX_OPERATIONS_HPP__
#define __MATRIX_OPERATIONS_HPP__

#include "./Matrix4.hpp"

#include "../Vectors/Vector3.hpp"

#include <cmath>

#include "../Vectors/Operations.hpp"

namespace MGL::Matrix::Operations
{

	inline Matrix4 Rotate(Matrix4 &m1, float angle, const Vectors::Vector3<float> &axis)
	{
		Matrix4 m2;
		m2.SetIdentity();

		float cosAngle = std::cos(angle);
		float sinAngle = std::sin(angle);
		float negatedcos = 1 - cosAngle;
		Vectors::Vector3<float> a = Vectors::Operations::Normalize(axis);

		m2(0, 0) = cosAngle + a[0] * a[0] * negatedcos;
		m2(0, 1) = a[0] * a[1] * negatedcos + a[2] * sinAngle;
		m2(0, 2) = a[0] * a[2] * negatedcos - a[1] * sinAngle;

		m2(1, 0) = a[1] * a[0] * negatedcos - a[2] * sinAngle;
		m2(1, 1) = cosAngle + a[1] * a[1] * negatedcos;
		m2(1, 2) = a[1] * a[2] * negatedcos + a[0] * sinAngle;

		m2(2, 0) = a[2] * a[0] * negatedcos + a[1] * sinAngle;
		m2(2, 1) = a[2] * a[1] * negatedcos - a[0] * sinAngle;
		m2(2, 2) = cosAngle + a[2] * a[2] * negatedcos;

		return m1 * m2;
	}

	inline Matrix4 LookAt(Vectors::Vector3<float> CPos, Vectors::Vector3<float> TPos, Vectors::Vector3<float> DUp)
	{
		Vectors::Vector3<float> F = Vectors::Operations::Normalize(TPos - CPos);
		Vectors::Vector3<float> S = Vectors::Operations::Normalize(Vectors::Operations::Cross(F, DUp));
		Vectors::Vector3<float> U = Vectors::Operations::Cross(S, F);

		Matrix4 m1;
		m1.SetIdentity();

		m1(0, 0) = S[0];
		m1(0, 1) = U[0];
		m1(0, 2) = -F[0];

		m1(1, 0) = S[1];
		m1(1, 1) = U[1];
		m1(1, 2) = -F[1];

		m1(2, 0) = S[2];
		m1(2, 1) = U[2];
		m1(2, 2) = -F[2];

		m1(3, 0) = -Vectors::Operations::Dot(S, CPos);
		m1(3, 1) = -Vectors::Operations::Dot(U, CPos);
		m1(3, 2) = Vectors::Operations::Dot(F, CPos);

		return m1;
	}

	inline Matrix4 Perspective(float fov, float aspect, float near, float far)
	{
		float f = 1.0f / std::tan(fov / 2.0f);

		Matrix4 m1;
		// m1.SetIdentity();
		m1(0, 0) = f / aspect;
		m1(1, 1) = -f; // Inversion de Y pour Vulkan

		m1(2, 2) = far / (far - near); // Vulkan: z in [0, 1]
		m1(2, 3) = 1.0f;

		m1(3, 2) = -(far * near) / (far - near);
		return m1;
	}

	inline Matrix4 Translate(Matrix4 &m1, Vectors::Vector3<float> dest)
	{
		Matrix4 m2(m1);

		m2(3, 0) += dest[0];
		m2(3, 1) += dest[1];
		m2(3, 2) += dest[2];

		return m2;
	}
} // namespace MGL::Matrix::Operations

#endif // __MATRIX_OPERATIONS_HPP__