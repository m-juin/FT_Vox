#ifndef __COLLIDER_HPP__
#define __COLLIDER_HPP__

#include "./Frustrum.hpp"

namespace Vox::Front::Rendering::Frustrum::Colliders
{
    struct Collider
    {
        virtual bool IsOnFrustrum(const Frustrum &frustrum) = 0;
        virtual ~Collider() {};
    };
    
} // namespace Vox::Front::Rendering::Frustrum::Colliders


#endif // __COLLIDER_HPP__