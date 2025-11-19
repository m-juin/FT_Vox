#include "Front/Rendering/Frustrum/BoxCollider.hpp"

namespace Vox::Front::Rendering::Frustrum::Colliders
{
	BoxCollider::BoxCollider(Frustrum::Vector3Float origin_, Frustrum::Vector3Float size)
		: origin(origin_), max(origin_ + size) {};

	bool BoxCollider::IsOnFrustrum(const Frustrum &frustrum)
	{
		// std::cout << "[DEBUG] min : " << this->origin << " | max: " << this->max << std::endl;
		return (IsOnForwardPlane(frustrum.left) && IsOnForwardPlane(frustrum.right) && IsOnForwardPlane(frustrum.top) &&
				IsOnForwardPlane(frustrum.bot) && IsOnForwardPlane(frustrum.near) && IsOnForwardPlane(frustrum.far));
	}

	bool BoxCollider::IsOnForwardPlane(const Frustrum::FrustrumPlane &plane)
	{
		Frustrum::Vector3Float positiveVertex = this->max; // Commence avec max

		for (uint8_t index = 0; index < 3; index++)
			if (plane.normal[index] < 0) // Si normale négative, prendre min
				positiveVertex[index] = this->origin[index];

		return MGL::Vectors::Operations::Dot(plane.normal, positiveVertex) - plane.distance >= 0;
	}
} // namespace Vox::Front::Rendering::Frustrum::Colliders