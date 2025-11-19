#ifndef __BOXCOLLIDER_HPP__
#define __BOXCOLLIDER_HPP__

#include "./Collider.hpp"

namespace Vox::Front::Rendering::Frustrum::Colliders
{
    struct BoxCollider : public Collider
    {
        Frustrum::Vector3Float origin;
        Frustrum::Vector3Float max;

        BoxCollider(Frustrum::Vector3Float, Frustrum::Vector3Float);

        bool IsOnFrustrum(const Frustrum &frustrum) override;
        bool IsOnForwardPlane(const Frustrum::FrustrumPlane &plane);

        ~BoxCollider() {};
    };
    
}

#endif // __BOXCOLLIDER_HPP__