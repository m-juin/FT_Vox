#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include "./Vector2.hpp"
#include "./Vector3.hpp"

namespace MGL::Vectors::Types
{
using Vector2Int = Vector2<int>;
using Vector3Int = Vector3<int>;

using Vector2float = Vector2<float>;
using Vector3float = Vector3<float>;

using Vector2uint8 = Vector2<uint8_t>;
using Vector3uint8 = Vector3<uint8_t>;

} // namespace MGL::Vectors

#endif // __DEFINES_HPP__