#ifndef __VECTOR2_HPP__
#define __VECTOR2_HPP__

#include <iostream>

namespace MGL::Vectors
{
	template <typename T> struct Vector2
	{
		public:
			Vector2(T x, T y) : m{x, y} {}
			Vector2(T val) : m{val, val} {}
			Vector2() : m{T(), T()} {}
			Vector2(const Vector2 &v1) : m{v1.m[0], v1.m[1]} {}
            
			bool operator==(const Vector2<T> &m1) const
			{
				return this->m[0] == m1.m[0] && this->m[1] == m1.m[1];
			};
			bool operator!=(const Vector2<T> &m1) const
			{
				return this->m[0] != m1.m[0] || this->m[1] != m1.m[1];
			};
			void operator=(const Vector2<T> &m1)
			{
				this->m[0] = m1.m[0];
				this->m[1] = m1.m[1];
			};
			void operator+=(const Vector2<T> &m1)
			{
				this->m[0] += m1.m[0];
				this->m[1] += m1.m[1];
			};

        T &operator[](size_t pos) {return this->m[pos];}
        const T &operator[](size_t pos) const {return this->m[pos];}

        public:
            T m[2];
	};

	template <typename T> std::ostream &operator<<(std::ostream &os, const Vector2<T> &dt)
	{
		os << "Vector2(" << dt.m[0] << ", " << dt.m[1] << ")";
		return os;
	}
} // namespace MGL::Vectors

#endif // __VECTOR2_HPP__