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
			Matrix4(const double m_[16])
			{
				for (size_t i = 0; i < 16; i++)
					m[i] = m_[i];
			};
			~Matrix4() {};

			double &operator()(size_t row, size_t col)
			{
				return m[row * 4 + col];
			}

			const double &operator()(size_t row, size_t col) const
			{
				return m[row * 4 + col];
			}

			void SetIdentity()
			{
				for (size_t i = 0; i < 16; i++)
				{
					m[i] = 0.0;
				}
				m[15] = 1.0;
			}

			Matrix4 operator*(const Matrix4 &m1) const
			{
				Matrix4 result;
				for (size_t i = 0; i < 4; i++)
				{
					for (size_t j = 0; j < 4; j++)
					{
						result(i, j) = 0.0;
						for (size_t k = 0; k < 4; k++)
						{
							result(i, j) += (*this)(i, k) * m1(k, j);
						}
					}
				}
				return result;
			}

			Vectors::Vector4<float> operator*(const Vectors::Vector4<float> &v) const
			{
				Vectors::Vector4<float> result;
				for (size_t i = 0; i < 4; i++)
				{
					result[i] = 0.0;
					for (size_t j = 0; j < 4; j++)
					{
						result[i] += (*this)(i, j) * v[j];
					}
				}
				return result;
			}

		private:
			double m[16];
	};

	std::ostream &operator<<(std::ostream &os, const Matrix4 &dt)
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

	Vectors::Vector4<double> operator*(const Matrix::Matrix4 &m1, const Vectors::Vector4<double> &v)
	{
		Vectors::Vector4<double> result;
		for (size_t i = 0; i < 4; i++)
		{
			result[i] = 0.0;
			for (size_t j = 0; j < 4; j++)
			{
				result[i] += (m1)(i, j) * v[j];
			}
		}
		return result;
	}

} // namespace MGL::Matrix
#endif // __MATRIX4_HPP__