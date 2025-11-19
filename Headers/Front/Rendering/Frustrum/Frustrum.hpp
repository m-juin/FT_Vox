#ifndef __FRUSTRUM_HPP__
#define __FRUSTRUM_HPP__

#include "MathGraphicalLib/Vectors/Operations.hpp"

namespace Vox::Front::Rendering::Frustrum
{
	struct Frustrum
	{
			using Vector3Float = MGL::Vectors::Vector3<float>;
			struct FrustrumPlane
			{
					Vector3Float normal;
					float distance;

					FrustrumPlane(const Vector3Float &point, const Vector3Float &normal_)
					{
						normal = MGL::Vectors::Operations::Normalize(normal_);
						distance = MGL::Vectors::Operations::Dot(normal, point);
					}
					FrustrumPlane() {};
			};

			FrustrumPlane top;
			FrustrumPlane bot;

			FrustrumPlane right;
			FrustrumPlane left;

			FrustrumPlane far;
			FrustrumPlane near;
	};
} // namespace Vox::Front::Rendering::Frustrum
#endif // __FRUSTRUM_HPP__