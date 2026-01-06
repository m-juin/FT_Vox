#ifndef __POISSONDISKSAMPLING_HPP__
#define __POISSONDISKSAMPLING_HPP__

#include <cmath>
#include <list>
#include <vector>

// #include "MathGraphicalLib/Vectors/Vector2.hpp"
#include "MathGraphicalLib/Vectors/Defines.hpp"

#include "MathGraphicalLib/Utils.hpp"

#include "Game/Scenes/World/Generation/Utils.hpp"

namespace Vox::World::Generation::Decorations
{
	using namespace MGL::Vectors::Types;

	static bool IsValid(Vector2Float candidate, float sqrRadius, Vector2Int region, int gridWidth, int gridHeight,
						float cellSize, std::vector<Vector2Float> &points, std::vector<int> &grid)
	{
		if (candidate[0] >= 0 && candidate[0] < region[0] && candidate[1] >= 0 && candidate[1] < region[1])
		{
			int cellX = static_cast<int>(candidate[0] / cellSize);
			int cellY = static_cast<int>(candidate[1] / cellSize);

			int searchStartX = std::max(0, cellX - 2);
			int searchEndX = std::min(cellX + 2, gridWidth - 1);
			int searchStartY = std::max(0, cellY - 2);
			int searchEndY = std::min(cellY + 2, gridHeight - 1);

			for (int x = searchStartX; x <= searchEndX; x++)
			{
				for (int y = searchStartY; y <= searchEndY; y++)
				{
					int index = grid[x + y * gridWidth];
					if (index >= 0 && index < static_cast<int>(points.size()))
					{
						Vector2Float point = points[index];
						float dx = candidate[0] - point[0];
						float dy = candidate[1] - point[1];
						float dst = dx * dx + dy * dy;
						if (dst < sqrRadius)
							return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	inline std::vector<Vector2Float> GenerateDiskTree(uint64_t seed, float radius = 2.f,
												 Vector2Int region = {Game::Utils::Defines::CHUNCK_SIZE,
																	  Game::Utils::Defines::CHUNCK_SIZE},
												 size_t tryBeforeRejection = 30)
	{
		std::mt19937 gen(seed);
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);

		float cellSize = radius / std::sqrt(2);
		float sqrRadius = radius * radius;

		int gridWidth = static_cast<int>(std::ceil(region[0] / cellSize));
		int gridHeight = static_cast<int>(std::ceil(region[1] / cellSize));
		int gridSize = gridWidth * gridHeight;

		std::vector<int> grid(gridSize, 0);
		std::vector<Vector2Float> points;
		std::vector<Vector2Float> spawnPoints;

		spawnPoints.push_back({region[0] / 2.f, region[1] / 2.f});

		while (!spawnPoints.empty())
		{
			size_t spawnIndex = static_cast<size_t>(dist(gen) * spawnPoints.size());
			Vector2Float spawnCenter = spawnPoints[spawnIndex];
			bool candidateAccepted = false;

			for (size_t i = 0; i < tryBeforeRejection; i++)
			{
				float angle = dist(gen) * M_PI * 2;
				Vector2Float dir = {std::sin(angle), std::cos(angle)};
				float distance = radius + dist(gen) * radius;
				Vector2Float candidate = {spawnCenter[0] + dir[0] * distance, spawnCenter[1] + dir[1] * distance};

				if (IsValid(candidate, sqrRadius, region, gridWidth, gridHeight, cellSize, points, grid))
				{
					points.push_back(candidate);
					spawnPoints.push_back(candidate);

					int candidateCellX = static_cast<int>(candidate[0] / cellSize);
					int candidateCellY = static_cast<int>(candidate[1] / cellSize);
					grid[candidateCellX + candidateCellY * gridWidth] = static_cast<int>(points.size() - 1);

					candidateAccepted = true;
					break;
				}
			}

			if (!candidateAccepted)
			{
				spawnPoints.erase(spawnPoints.begin() + spawnIndex);
			}
		}
		return points;
	}
	/// @brief Generate a PoissonDisk based on multiple factors annd inside a particular region
	/// @param worldChunck the target world chunck
	/// @param seed the Seed of the world + the hash of the region
	/// @param radius radius between each tree
	/// @param regionRadius the number of chunck around the target one
	/// @param tryBeforeRejection the try amount to find a suitable spawnPoint
	/// @return
inline std::vector<Vector2Float> GetChunckDiskSampling(
    Vector2Int worldChunk, 
    uint64_t seed, 
    float radius = 2.f, 
    size_t regionRadius = 2,
    size_t tryBeforeRejection = 30)
{
    int regionSize = static_cast<int>(regionRadius * 2 + 1);
    
    
    Vector2Int chunkInRegion = {
        (worldChunk[0] % regionSize + regionSize) %regionSize,
        (worldChunk[1] % regionSize + regionSize) %regionSize
    };
    
	int regionSizeInBlock = static_cast<int>(regionSize * Game::Utils::Defines::CHUNCK_SIZE);
    
    auto regionTrees = GenerateDiskTree(seed, radius, {regionSizeInBlock, regionSizeInBlock}, tryBeforeRejection);
    
    Vector2Int chunkStartInRegion = {
        static_cast<int>(chunkInRegion[0] * Game::Utils::Defines::CHUNCK_SIZE),
        static_cast<int>(chunkInRegion[1] * Game::Utils::Defines::CHUNCK_SIZE)
    };
    
    std::vector<Vector2Float> localTrees;
    for (const auto& tree : regionTrees) 
    {
        if (tree[0] >= chunkStartInRegion[0] && 
            tree[0] < chunkStartInRegion[0] + Game::Utils::Defines::CHUNCK_SIZE &&
            tree[1] >= chunkStartInRegion[1] && 
            tree[1] < chunkStartInRegion[1] + Game::Utils::Defines::CHUNCK_SIZE) 
        {
            localTrees.push_back({
                tree[0] - chunkStartInRegion[0],
                tree[1] - chunkStartInRegion[1]
            });
        }
    }
    
    return localTrees;
}

	inline uint64_t GetRegionnedSeed(uint64_t worldSeed, Vector2Int chunckPos, uint8_t regionRadius)
	{
		size_t regionSize = regionRadius * 2 + 1;
		Vector2Int worldRegion = {static_cast<int>(chunckPos[0] / regionSize), static_cast<int>(chunckPos[1] / regionSize)};
		size_t regionSeed = worldSeed + MGL::Vectors::Vector2Hash<int>()(worldRegion);
		return regionSeed;
	}

#include "Utils/Images/PutPixels.hpp"

	inline std::vector<uint8_t> GenerateDiskImage(uint64_t seed, uint16_t imgSize)
	{
		using namespace Vox::Utils::Images;

		auto tree = GenerateDiskTree(seed, 4, {16, 16}, 15);

		std::vector<uint8_t> img(imgSize * imgSize * 4, 255);

		MGL::Vectors::Vector2<size_t> vImgSize = {imgSize, imgSize};

		for (auto treePos : tree)
		{
			for (int x = -1; x <= 1; x++)
			{
				for (int y = -1; y <= 1; y++)
				{
					Vector2Int effectiveCoord = {static_cast<int>(treePos[0] * 25 + x),
																 static_cast<int>(treePos[1] * 25 + y)};
					if (effectiveCoord[0] >= 0 && effectiveCoord[0] < 400 && effectiveCoord[1] >= 0 &&
						effectiveCoord[1] < 400)
					{
						PutPixel(img, vImgSize,
								 {static_cast<size_t>(effectiveCoord[0]), static_cast<size_t>(effectiveCoord[1])},
								 false, {255, 0, 0});
					}
				}
			}
		}

		return img;
	}
} // namespace Vox::World::Generation::Decorations

// https://www.youtube.com/watch?v=7WcmyxyFO7o

#endif // __POISSONDISKSAMPLING_HPP__