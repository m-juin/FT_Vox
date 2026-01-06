#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include "./Vector2.hpp"
#include "./Vector3.hpp"

#include <cstdint>

namespace MGL::Vectors::Types
{
using Vector2Int = Vector2<int>;
using Vector3Int = Vector3<int>;

using Vector2Float = Vector2<float>;
using Vector3Float = Vector3<float>;

using Vector2Uint8 = Vector2<uint8_t>;
using Vector3Uint8 = Vector3<uint8_t>;

using Vector2SizeT = Vector2<size_t>;
using Vector3SizeT = Vector3<size_t>;

} // namespace MGL::Vectors

#endif // __DEFINES_HPP__