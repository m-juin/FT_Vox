#ifndef __BOXCOLLIDER_HPP__
#define __BOXCOLLIDER_HPP__

#include "./Collider.hpp"

namespace Vox::Front::Rendering::Frustum::Colliders
{
    struct BoxCollider : public Collider
    {
        Frustum::Vector3Float origin;
        Frustum::Vector3Float max;

        BoxCollider(Frustum::Vector3Float, Frustum::Vector3Float);

        bool IsOnFrustum(const Frustum &Frustum) override;
        bool IsOnForwardPlane(const Frustum::FrustumPlane &plane);

        ~BoxCollider() {};
    };
    
}

#endif // __BOXCOLLIDER_HPP__