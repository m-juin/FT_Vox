#ifndef __UTILS_HPP__
#define __UTILS_HPP__

namespace MGL::Utils
{
	template <typename T> T findNextMultiple(T number, T multiple_of)
	{
		if (multiple_of == 0)
		{
			throw std::invalid_argument("Division par zéro impossible");
		}

		// Calcul du quotient arrondi au prochain multiple
		auto quotient = (number + multiple_of - 1) / multiple_of;

		// Retourne le prochain multiple
		return quotient * multiple_of;
	}
} // namespace MGL::Utils

#endif // __UTILS_HPP__