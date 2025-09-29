#ifndef __MGL_UTILS_HPP__
#define __MGL_UTILS_HPP__

namespace MGL::Utils
{
	template <typename T> T findNextMultiple(T number, T multiple_of)
	{
		if (multiple_of == 0)
		{
			throw std::invalid_argument("Division par zéro impossible");
		}

		auto quotient = (number + multiple_of - 1) / multi
		return quotient * multiple_of;
	}

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
	inline float Radians(const float &degree)
	{
		return degree * (M_PI / 180);
	}
} // namespace MGL::Utils

#endif // __MGL_UTILS_HPP__