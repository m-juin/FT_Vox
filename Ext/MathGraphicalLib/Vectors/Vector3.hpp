#ifndef __VECTOR3_HPP__
#define __VECTOR3_HPP__

#include <iostream>

namespace MGL::Vectors
{
	template <typename T> struct Vector3
	{
		public:
			Vector3(T x, T y, T z) : m{x, y, z} {}
			Vector3(T val) : m{val, val, val} {}
			Vector3() : m{T(), T(), T()} {}
			Vector3(const Vector3 &v1) : m{v1[0], v1[1], v1[2]} {}

			bool operator==(const Vector3<T> &m1) const
			{
				return this->m[0] == m1[0] && this->m[1] == m1[1] && this->m[2] == m1[2];
			};
			bool operator!=(const Vector3<T> &m1) const
			{
				return this->m[0] != m1[0] || this->m[1] != m1[1] || this->m[2] != m1[2];
			};
			void operator=(const Vector3<T> &m1)
			{
				this->m[0] = m1[0];
				this->m[1] = m1[1];
				this->m[2] = m1[2];
			};
			void operator+=(const Vector3<T> &m1)
			{
				this->m[0] += m1[0];
				this->m[1] += m1[1];
				this->m[2] += m1[2];
			};
			void operator*=(const Vector3<T> &m1)
			{
				this->m[0] *= m1[0];
				this->m[1] *= m1[1];
				this->m[2] *= m1[2];
			};
			void operator/=(const Vector3<T> &m1)
			{
				this->m[0] /= m1[0];
				this->m[1] /= m1[1];
				this->m[2] /= m1[2];
			};
			void operator/=(const T &m1)
			{
				this->m[0] /= m1;
				this->m[1] /= m1;
				this->m[2] /= m1;
			};
			Vector3<T> operator/(const T &m1)
			{
				return {this->m[0] / m1, this->m[1] / m1, this->m[2] / m1};
			};
			void operator*=(T mult)
			{
				this->m[0] *= mult;
				this->m[1] *= mult;
				this->m[2] *= mult;
			};
			Vector3<T> operator-() const
			{
				return Vector3<T>(-m[0], -m[1], -m[2]);
			}
			T &operator[](size_t pos)
			{
				return this->m[pos];
			}
			T operator[](size_t pos) const
			{
				return this->m[pos];
			}

		private:
			T m[3];
	};

	template <typename T> Vector3<T> operator+(const Vector3<T> &m1, const Vector3<T> &m2)
	{
		return Vector3<T>(m1[0] + m2[0], m1[1] + m2[1], m1[2] + m2[2]);
	}

	template <typename T> Vector3<T> operator-(const Vector3<T> &m1, const Vector3<T> &m2)
	{
		return Vector3<T>(m1[0] - m2[0], m1[1] - m2[1], m1[2] - m2[2]);
	}

	template <typename T> Vector3<T> operator*(const Vector3<T> &m1, T const &mult)
	{
		return Vector3<T>(m1[0] * mult, m1[1] * mult, m1[2] * mult);
	}

	template <typename T> std::ostream &operator<<(std::ostream &os, const Vector3<T> &dt)
	{
		os << "Vector3(" << +dt[0] << ", " << +dt[1] << ", " << +dt[2] << ")";
		return os;
	}
	template <typename T> Vector3<T> operator*(T const &mult, const Vector3<T> &m1)
	{
		return Vector3<T>(m1[0] * mult, m1[1] * mult, m1[2] * mult);
	}
	template <typename T> struct Vector3Hash
	{
			std::size_t operator()(const Vector3<T> &v) const noexcept
			{
				std::size_t seed = 0;
				seed ^= std::hash<T>{}(v[0]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				seed ^= std::hash<T>{}(v[1]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				seed ^= std::hash<T>{}(v[2]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
				return seed;
			}
	};
} // namespace MGL::Vectors

#endif // __VECTOR3_HPP__