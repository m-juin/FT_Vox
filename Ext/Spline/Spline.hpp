#ifndef __SPLINE_HPP__
#define __SPLINE_HPP__

#include <cstring>
#include <fstream>
#include <vector>

#include "Range.hpp"

namespace Spline
{
	inline double GetNormalizedRangedValue(float initValue, std::pair<double, double> initialRange, std::pair<double, double> targetRange)
		{
			return targetRange.first +
				   (initValue - initialRange.first) * (targetRange.second - targetRange.first) / (initialRange.second - initialRange.first);
		}

	class Spline
	{
		public:
			Spline(Range range_, std::vector<std::pair<double, double>> points_) : range(range_), points(points_) {};
			~Spline() {};

			const Range range;

			Spline(const Spline &src) : range(src.range), points(src.points) {};
			Spline(Spline &src) : range(src.range), points(src.points) {};

			const std::vector<std::pair<double, double>> points;

			double GetValue(double originalValue) const
			{
				if (points.size() == 0) return 0.0;
				for (size_t index = 0; index < this->points.size(); index++)
				{
					if (index + 1 == this->points.size()) return this->points[index].second;
					if (index == 0 && this->points[index].first > originalValue) return this->points[0].second;
					if (points[index].first >= originalValue)
					{
						double prev = points[index - 1 ].second;
						double next = points[index].second;

						return GetNormalizedRangedValue(originalValue, {points[index - 1].first, points[index].first}, {prev, next});
					}
				}
				return 0.0;
			}
	};

	static inline const Spline *LoadSpline(const char *path)
	{
		std::ifstream file(path);
		if (file.is_open() == false)
			throw std::runtime_error("Failed to open spline file.");

		std::string line;
		int step = 0;

		std::pair<double, double> horizontal;
		std::pair<double, double> vertical;

		std::vector<std::pair<double, double>> pointList;

		try
		{
			std::pair<double, double> curpair;

			while (std::getline(file, line))
			{
				if (step == 0 && line.find("Range") != line.npos)
					step = 1;
				else if (step == 1 && line.find("{") != line.npos)
					step = 2;
				else if (step == 2)
				{
					if (line.find("xMin: ") != line.npos)
						horizontal.first = std::stod(line.substr(line.find("xMin: ") + 5));
					else if (line.find("xMax: ") != line.npos)
						horizontal.second = std::stod(line.substr(line.find("xMax: ") + 5));
					else if (line.find("yMin: ") != line.npos)
						vertical.first = std::stod(line.substr(line.find("yMin: ") + 5));
					else if (line.find("yMax: ") != line.npos)
						vertical.second = std::stod(line.substr(line.find("yMax: ") + 5));
					else if (line.find("}") != line.npos)
						step = 3;
				}
				else if (step == 3)
				{
					if (line.find("{") != line.npos)
						curpair = std::pair<double, double>();
					else if (line.find("x: ") != line.npos)
						curpair.first = std::stod(line.substr(line.find("x: ") + 2));
					else if (line.find("y: ") != line.npos)
						curpair.second = std::stod(line.substr(line.find("y: ") + 2));
					else if (line.find("}") != line.npos)
						pointList.push_back(curpair);
				}
			}
			Spline *spline = new Spline(Range(horizontal, vertical), pointList);
			return spline;
		}
		catch (std::exception e)
		{
			throw std::move(e);
		}
	}
} // namespace Spline

#endif // __SPLINE_HPP__