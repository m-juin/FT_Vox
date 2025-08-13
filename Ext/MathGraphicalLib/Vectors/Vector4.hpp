#ifndef __VECTOR4_HPP__
#define __VECTOR4_HPP__

#include <iostream>

namespace MGL::Vectors
{
	template <typename T> struct Vector4
	{
		public:
			Vector4(T x, T y, T z, T a) : m{x, y, z, a} {}
			Vector4(T val) : m{val, val, val, val} {}
			Vector4() : m{T(), T(), T(), T()} {}
			Vector4(const Vector4 &v1) : m{v1[0], v1[1], v1[2], v1[3]} {}

			bool operator==(const Vector4<T> &m1) const
			{
				return this->m[0] == m1[0] && this->m[1] == m1[1] && this->m[2] == m1[2] && this->m[3] == m1[3];
			};
			bool operator!=(const Vector4<T> &m1) const
			{
				return this->m[0] != m1[0] || this->m[1] != m1[1] || this->m[2] != m1[2] || this->m[3] != m1[3];
			};
			void operator=(const Vector4<T> &m1)
			{
				this->m[0] = m1[0];
				this->m[1] = m1[1];
				this->m[2] = m1[2];
				this->m[3] = m1[3];
			};
			void operator+=(const Vector4<T> &m1)
			{
				this->m[0] += m1[0];
				this->m[1] += m1[1];
				this->m[2] += m1[2];
				this->m[3] += m1[3];
			};
			void operator*=(Vector4<T> &m1)
			{
				this->m[0] *= m1[0];
				this->m[1] *= m1[1];
				this->m[2] *= m1[2];
				this->m[3] *= m1[3];
			};
			void operator*=(T mult)
			{
				this->m[0] *= mult;
				this->m[1] *= mult;
				this->m[2] *= mult;
				this->m[3] *= mult;
			};


            T &operator[](size_t pos) {return this->m[pos];}
            T operator[](size_t pos) const {return this->m[pos];}

        private:
            T m[4];
	};

	template <typename T> Vector4<T> operator+(const Vector4<T> &m1, const Vector4<T> &m2)
	{
		return Vector4<T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2], m1[3] + m2[3]);
	}

	template <typename T> Vector4<T> operator-(const Vector4<T> &m1, const Vector4<T> &m2)
	{
		return Vector4<T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2], m1[3] - m2[3]);
	}

	template <typename T> Vector4<T> operator*(const Vector4<T> &m1, T const &mult)
	{
		return Vector4<T>(m1[0] * mult, m1[1] * mult, m1[2] * mult, m1[3] * mult);
	}

	template <typename T> std::ostream &operator<<(std::ostream &os, const Vector4<T> &dt)
	{
		os << "Vector4(" << dt[0] << ", " << dt[1] << ", " << dt[2] << ", " << dt[3] << ")";
		return os;
	}
} // namespace MGL::Vectors

#endif // __VECTOR4_HPP__