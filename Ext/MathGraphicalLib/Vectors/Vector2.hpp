#ifndef __VECTOR2_HPP__
#define __VECTOR2_HPP__

#include <iostream>
#include <cmath>

namespace MGL::Vectors
{
	template<typename T>
	struct Vector2
	{
	public:
		constexpr Vector2(T x, T y) : m{x, y} {}
		constexpr Vector2(T val) : m{val, val} {}
		constexpr Vector2() : m{T(), T()} {}
		constexpr Vector2(const Vector2 &v1) : m{v1.m[0], v1.m[1]} {}

		bool operator==(const Vector2<T> &m1) const
		{
			return this->m[0] == m1.m[0] && this->m[1] == m1.m[1];
		};

		bool operator!=(const Vector2<T> &m1) const
		{
			return this->m[0] != m1.m[0] || this->m[1] != m1.m[1];
		};

		Vector2<T> &operator=(const Vector2<T> &m1)
		{
			this->m[0] = m1.m[0];
			this->m[1] = m1.m[1];
			return *this;
		}

		void operator+=(const Vector2<T> &m1)
		{
			this->m[0] += m1.m[0];
			this->m[1] += m1.m[1];
		};

		T &operator[](size_t pos) { return this->m[pos]; }
		const T &operator[](size_t pos) const { return this->m[pos]; }

	public:
		T m[2];
	};

	template<typename T>
	bool operator==(const Vector2<T> &m1, const Vector2<T> &m2)
	{
		return m1->m[0] == m2.m[0] && m1->m[1] == m2.m[1];
	};

	template<typename T>
	std::ostream &operator<<(std::ostream &os, const Vector2<T> &dt)
	{
		os << "Vector2(" << dt.m[0] << ", " << dt.m[1] << ")";
		return os;
	}
	template <typename T>
	double Dist(const Vector2<T> &d1, const Vector2<T> &d2)
	{
		double dx = static_cast<double>(d1[0] - d2[0]);
		double dy = static_cast<double>(d1[1] - d2[1]);
		return dx * dx + dy * dy;
	}
	template<typename T>
	struct Vector2Hash
	{
		std::size_t operator()(const Vector2<T> &v) const noexcept
		{
			std::size_t seed = 0;
			seed ^= std::hash<T>{}(v[0]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= std::hash<T>{}(v[1]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
} // namespace MGL::Vectors

#endif // __VECTOR2_HPP__
