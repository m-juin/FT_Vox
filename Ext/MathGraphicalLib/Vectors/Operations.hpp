#ifndef __VECTORS_OPERATIONS_HPP__
#define __VECTORS_OPERATIONS_HPP__

#include "./Vector3.hpp"

#include <cmath>

namespace MGL::Vectors::Operations
{
	inline Vector3<float> Normalize(const Vector3<float> &v1)
	{
		float length = std::sqrt((v1[0] * v1[0] + (v1[1] * v1[1]) + (v1[2] * v1[2])));

		return {v1[0] / length, v1[1] / length, v1[2] / length};
	}

	inline Vector3<float> Cross(const Vector3<float> &v1, const Vector3<float> &v2)
	{
		Vector3<float> ret;
		ret[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
		ret[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
		ret[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
		return ret;
	}

	inline float Dot(Vector3<float> const &v1, Vector3<float> const &v2)
	{
		return float((v1[0] * v2[0]) + (v1[1] * v2[1]) + (v1[2] * v2[2]));
	}
}

#endif // __VECTORS_OPERATIONS_HPP__