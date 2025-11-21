#ifndef __COLLIDER_HPP__
#define __COLLIDER_HPP__

#include "./Frustum.hpp"

namespace Vox::Front::Rendering::Frustum::Colliders
{
    struct Collider
    {
        virtual bool IsOnFrustum(const Frustum &Frustum) = 0;
        virtual ~Collider() {};
    };
    
} // namespace Vox::Front::Rendering::Frustum::Colliders


#endif // __COLLIDER_HPP__