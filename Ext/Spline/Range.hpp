#ifndef __RANGE_HPP__
#define __RANGE_HPP__

#include <utility>

namespace Spline
{
	struct Range
	{
		std::pair<double, double> horizontal;
		std::pair<double, double> vertical;

		Range(std::pair<double, double> horizontal_, std::pair<double, double> vertical_)
				: horizontal(horizontal_), vertical(vertical_) {};

        Range(const Range &src)
        {
            this->horizontal = src.horizontal;
            this->vertical = src.vertical;
        }

        Range& operator=(const Range& newRange)
        {
            if (this == &newRange)
                return *this;
            this->horizontal = newRange.horizontal;
            this->vertical = newRange.vertical;
            return *this;
        }

        private:
            Range();
	};

} // namespace Spline

#endif // __RANGE_HPP__