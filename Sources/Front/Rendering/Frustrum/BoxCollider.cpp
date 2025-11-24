#include "Front/Rendering/Frustum/BoxCollider.hpp"

#include <cstdint>

namespace Vox::Front::Rendering::Frustum::Colliders
{
	BoxCollider::BoxCollider(Frustum::Vector3Float origin_, Frustum::Vector3Float size)
		: origin(origin_), max(origin_ + size) {};

	bool BoxCollider::IsOnFrustum(const Frustum &Frustum)
	{
		return (IsOnForwardPlane(Frustum.left) && IsOnForwardPlane(Frustum.right) && IsOnForwardPlane(Frustum.top) &&
				IsOnForwardPlane(Frustum.bot) && IsOnForwardPlane(Frustum.near) && IsOnForwardPlane(Frustum.far));
	}
	bool BoxCollider::IsOnForwardPlane(const Frustum::FrustumPlane &plane)
	{
		Frustum::Vector3Float positiveVertex = this->max;

		for (uint8_t index = 0; index < 3; index++)
			if (plane.normal[index] < 0)
				positiveVertex[index] = this->origin[index];

		return MGL::Vectors::Operations::Dot(plane.normal, positiveVertex) + plane.distance >= 0;
	}
} // namespace Vox::Front::Rendering::Frustum::Colliders