#ifndef __MATRIX4_HPP__
#define __MATRIX4_HPP__

#include "../Vectors/Vector4.hpp"

namespace MGL::Matrix
{
	class Matrix4
	{
		public:
			Matrix4()
			{
				this->SetIdentity();
			};
			Matrix4(const float m_[16])
			{
				for (size_t i = 0; i < 16; i++)
					m[i] = m_[i];
			};
			Matrix4(const Matrix4 &m1)
			{
				if (&m1 == this)
					return ;
				for (size_t i = 0; i < 16; i++)
					m[i] = m1.m[i];
			}
			~Matrix4() {};

			inline float &operator()(size_t row, size_t col)
			{
				return m[row * 4 + col];
			}

			const float &operator()(size_t row, size_t col) const
			{
				return m[row * 4 + col];
			}

			inline void SetIdentity()
			{
				for (size_t c = 0; c < 4; c++)
					for (size_t l = 0; l < 4; l++)
							this->m[c * 4 + l] = c == l;
			}

			inline Matrix4 operator*(const Matrix4 &m1) const
			{
				Matrix4 result;
				for (size_t i = 0; i < 4; i++)
				{
					for (size_t j = 0; j < 4; j++)
					{
						result(i, j) = 0.0f;
						for (size_t k = 0; k < 4; k++)
						{
							result(i, j) += (*this)(i, k) * m1(k, j);
						}
					}
				}
				return result;
			}

			inline Vectors::Vector4<float> operator*(const Vectors::Vector4<float> &v) const
			{
				Vectors::Vector4<float> result;
				for (size_t i = 0; i < 4; i++)
				{
					result[i] = 0.0f;
					for (size_t j = 0; j < 4; j++)
					{
						result[i] += (*this)(i, j) * v[j];
					}
				}
				return result;
			}

		private:
			float m[16];
	};

	inline std::ostream &operator<<(std::ostream &os, const Matrix4 &dt)
	{
		os << "\n\nMatrix4: " << &dt << "\n";
		for (size_t y = 0; y < 4; y++)
		{
			for (size_t x = 0; x < 4; x++)
			{
				std::cout << dt(y, x) << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		return os;
	}

	inline Vectors::Vector4<float> operator*(const Matrix::Matrix4 &m1, const Vectors::Vector4<float> &v)
	{
		Vectors::Vector4<float> result;
		for (size_t i = 0; i < 4; i++)
		{
			result[i] = 0.0f;
			for (size_t j = 0; j < 4; j++)
			{
				result[i] += (m1)(i, j) * v[j];
			}
		}
		return result;
	}

} // namespace MGL::Matrix
#endif // __MATRIX4_HPP__