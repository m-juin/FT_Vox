#ifndef __MATHS_HPP__
#define __MATHS_HPP__

#include "MathGraphicalLib/Vectors/Defines.hpp"

namespace Vox::Front::Interfaces::Utils::Maths
{
    using namespace MGL::Vectors::Types;
    inline Vector2Float PointPixelToVulkan(const Vector2Float point, const Vector2Float screenSize)
    {
        Vector2Float ret;
        ret[0] = (((float)point[0] / screenSize[0]) * 2.0) - 1.0f;
        ret[1] = (((float)point[1] / screenSize[1]) * 2.0) - 1.0f;
        return ret;
    }
} // namespace Vox::Front::Interfaces::Utils::Maths


#endif // __MATHS_HPP__