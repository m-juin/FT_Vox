#ifndef __VECTORS_OPERATIONS_HPP__
#define __VECTORS_OPERATIONS_HPP__

#include "./Vector3.hpp"
#include <cmath>
#include <limits>

namespace MGL::Vectors::Operations
{
    inline float Dot(const Vector3<float> &v1, const Vector3<float> &v2)
    {
        return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
    }

    inline float LengthSquared(const Vector3<float> &v)
    {
        return Dot(v, v);
    }

    inline float Length(const Vector3<float> &v)
    {
        return std::sqrt(LengthSquared(v));
    }

    inline Vector3<float> Normalize(const Vector3<float> &v)
    {
        float len = Length(v);
        if (len <= std::numeric_limits<float>::epsilon()) {
            return Vector3<float>(0.0f); // Retourne un vecteur nul plutôt que de diviser par zéro
        }
        return Vector3<float>(v[0] / len, v[1] / len, v[2] / len);
    }

    inline Vector3<float> Cross(const Vector3<float> &v1, const Vector3<float> &v2)
    {
        return Vector3<float>(
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]
        );
    }

    // Fonctions utilitaires supplémentaires
    inline float Distance(const Vector3<float> &v1, const Vector3<float> &v2)
    {
        return Length(v2 - v1);
    }

    inline float DistanceSquared(const Vector3<float> &v1, const Vector3<float> &v2)
    {
        Vector3<float> diff = v2 - v1;
        return LengthSquared(diff);
    }

} // namespace MGL::Vectors::Operations

#endif // __VECTORS_OPERATIONS_HPP__