#ifndef __POISSONDISKSAMPLING_HPP__
#define __POISSONDISKSAMPLING_HPP__

#include <cmath>
#include <list>
#include <vector>

#include "MathGraphicalLib/Vectors/Vector2.hpp"

#include "Game/Scenes/World/Generation/Utils.hpp"

namespace Vox::World::Generation::Decorations
{
	using Vector2 = MGL::Vectors::Vector2<float>;
	using Vector2Int = MGL::Vectors::Vector2<int>;

	static bool IsValid(Vector2 candidate, float sqrRadius, Vector2Int region, float cellSize,
						std::vector<Vector2> &points, std::vector<int> &grid)
	{
		if (candidate[0] >= 0 && candidate[0] < region[0] && candidate[1] >= 0 && candidate[1] < region[1])
		{

			int gridX = static_cast<int>(std::ceil(region[0] / cellSize));
			int gridY = static_cast<int>(std::ceil(region[1] / cellSize));

			int cellX = static_cast<int>(candidate[0] / cellSize);
			int cellY = static_cast<int>(candidate[1] / cellSize);
			int searchStartX = std::max(0, cellX - 2);
			int searchEndX = std::min(cellX + 2, gridX - 1);
			int searchStartY = std::max(0, cellY - 2);
			int searchEndY = std::min(cellY + 2, gridY - 1);

			for (int x = searchStartX; x <= searchEndX; x++)
			{
				for (int y = searchStartY; y <= searchEndY; y++)
				{
					int index = grid[x + y * gridX] - 1;

					if (index != -1)
					{
						auto point = points[index];
						auto vec = Vector2(candidate[0] - point[0], candidate[1] - point[1]);
						float dst = vec[0] * vec[0] + vec[1] * vec[1];
						if (dst < sqrRadius)
							return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	inline std::vector<Vector2> GenerateDiskTree(uint32_t seed, float radius = 2.f,
												 Vector2Int region = {Game::Utils::Defines::CHUNCK_SIZE,
																	  Game::Utils::Defines::CHUNCK_SIZE},
												 size_t tryBeforeRejection = 30)
	{
		std::mt19937 gen(seed);
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);

		float cellSize = radius / std::sqrt(2);
		float sqrRadius = radius * radius;

		int gridX = static_cast<int>(std::ceil(region[0] / cellSize));
		int gridY = static_cast<int>(std::ceil(region[1] / cellSize));
		int gridSize = gridX * gridY;
		std::vector<int> grid(gridSize, -1);
		std::vector<Vector2> points;
		std::vector<Vector2> spawnPoints;

		spawnPoints.push_back({region[0] / 2.f, region[1] / 2.f});
		while (spawnPoints.empty() == false)
		{
			size_t index = static_cast<size_t>(dist(gen) * spawnPoints.size());
			Vector2 sCenter = spawnPoints[index];
			bool accepted = false;

			for (size_t i = 0; i < tryBeforeRejection; i++)
			{
				float angle = dist(gen) * M_PI * 2;
				Vector2 dir = {std::sin(angle), std::cos(angle)};
				float mult = radius + dist(gen) * radius;
				Vector2 candidate = {sCenter[0] + (dir[0] * mult), sCenter[1] + (dir[1] * mult)};
				if (IsValid(candidate, sqrRadius, region, cellSize, points, grid))
				{
					points.push_back(candidate);
					spawnPoints.push_back(candidate);
					int gridX = static_cast<int>(candidate[0] / cellSize);
					int gridY = static_cast<int>(candidate[1] / cellSize);
					grid[gridX + gridY * gridX] = static_cast<int>(points.size() - 1);
					accepted = true;
					break;
				}
			}
			if (accepted == false)
				spawnPoints.erase(spawnPoints.begin() + index);
		}
		return points;
	}

	#include "Utils/Images/PutPixels.hpp"

	inline std::vector<uint8_t> GenerateDiskImage(uint32_t seed,
								  uint16_t imgSize)
	{
		// uint16_t halfSize = imgSize / 2;

		using namespace Vox::Utils::Images;

		auto tree = GenerateDiskTree(seed, 2., {400, 400}, 15);

		std::vector<uint8_t> img(imgSize * imgSize * 4, 255);

		MGL::Vectors::Vector2<size_t> vImgSize = {imgSize, imgSize};

		for (auto treePos : tree)
		{
			for (size_t x = -1; x <= 1; x++)
			{
				for (size_t y = -1; y <= 1; y++)
				{
					MGL::Vectors::Vector2<int> effectiveCoord = {static_cast<int>(treePos[0] + x), static_cast<int>(treePos[1] + y)};
					if (effectiveCoord[0] >= 0 && treePos[0] < 400 && effectiveCoord[1] >= 0 && treePos[1] < 400)
						PutPixel(img, vImgSize, {static_cast<size_t>(effectiveCoord[0]), static_cast<size_t>(effectiveCoord[1])}, false, {255, 0, 0});
				}
			}
		}
		return img;
	}
} // namespace Vox::World::Generation::Decorations

// https://www.youtube.com/watch?v=7WcmyxyFO7o

#endif // __POISSONDISKSAMPLING_HPP__