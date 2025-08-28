#ifndef __MATHS_HPP__
#define __MATHS_HPP__

#include "MathGraphicalLib/Vectors/Vector2.hpp"

namespace Vox::Front::Interfaces::Utils::Maths
{
    inline MGL::Vectors::Vector2<float> PointPixelToVulkan(const MGL::Vectors::Vector2<float> point, const MGL::Vectors::Vector2<float> screenSize)
    {
        MGL::Vectors::Vector2<float> ret;
        ret[0] = (((float)point[0] / screenSize[0]) * 2.0) - 1.0f;
        ret[1] = (((float)point[1] / screenSize[1]) * 2.0) - 1.0f;
        return ret;
    }
} // namespace Vox::Front::Interfaces::Utils::Maths


#endif // __MATHS_HPP__