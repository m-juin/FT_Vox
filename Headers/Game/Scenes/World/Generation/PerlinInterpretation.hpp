#ifndef __PERLININTERPRETATION_HPP__
#define __PERLININTERPRETATION_HPP__

#include "./BiomesPerlin.hpp"
#include "./Perlin.hpp"
#include "Spline/Spline.hpp"

#include "./Utils.hpp"

namespace Vox::Game::Generation::Perlins
{
	inline float GetBlendedHeightAt(int x, int z, uint32_t seed,
									const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl,
									int blendRadius = 4)
	{
		// Valeur de base commune à tous
		size_t xPos = x + WORLD_CENTER;
		size_t zPos = z + WORLD_CENTER;

		float contVal =
			spl.at("Continental")
				.first.GetValue(GetPerlinValue(xPos, zPos, seed, Utils::ContinentalnessData, {-1.2f, 1.0f}));

		float eroVal =
			spl.at("Erosion").first.GetValue(GetPerlinValue(xPos, zPos, seed, Utils::ErosionData, {-1.0f, 1.0f}));

		double PAVVal = GetPerlinValue(xPos, zPos, seed, Utils::PeaksAndValleyData, {-1.0f, 1.0f});
		PAVVal = 1 - std::abs(3 * std::abs(PAVVal) - 2);
		PAVVal = spl.at("P&V").first.GetValue(PAVVal);

		contVal *= spl.at("Continental").second;
		float baseHeight = contVal * eroVal;

		// --- Biome blending ---
		float weightedSum = 0.0f;
		float totalWeight = 0.0f;

		for (int dx = -blendRadius; dx <= blendRadius; dx++)
		{
			for (int dz = -blendRadius; dz <= blendRadius; dz++)
			{
				int bx = x + dx;
				int bz = z + dz;
				auto biome = Perlins::GetBiomeAtPoint(bx + WORLD_CENTER, bz + WORLD_CENTER, seed);

				// Distance au centre
				float dist2 = float(dx * dx + dz * dz);
				float weight = 1.0f / (dist2 + 1.0f); // +1 pour éviter div0

				// Hauteur avec la règle de ce biome
				float h = Game::Generation::Datas::Biomes::GetHeight(biome, baseHeight);

				weightedSum += h * weight;
				totalWeight += weight;
			}
		}

		float finalHeight = weightedSum / totalWeight;
		return static_cast<uint8_t>(finalHeight);
	}

	inline uint8_t GetHeightAt(int x, int z, uint32_t seed,
							   const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl)
	{
		// size_t xPos = x + WORLD_CENTER;
		// size_t zPos = z + WORLD_CENTER;
		// float contVal =
		// 	spl.at("Continental")
		// 		.first.GetValue(GetPerlinValue(xPos, zPos, seed, Utils::ContinentalnessData, {-1.2f, 1.0f}));

		// float eroVal =
		// 	spl.at("Erosion").first.GetValue(GetPerlinValue(xPos, zPos, seed, Utils::ErosionData, {-1.0f, 1.0f}));

		// double PAVVal = GetPerlinValue(xPos, zPos, seed, Utils::PeaksAndValleyData, {-1.0f, 1.0f});

		// PAVVal = 1 - std::abs(3 * std::abs(PAVVal) - 2);
		// PAVVal = spl.at("P&V").first.GetValue(PAVVal);
		// contVal *= spl.at("Continental").second;

		// auto biome = Perlins::GetBiomeAtPoint(xPos, zPos, seed);

		// // eroVal *= spl.at("Erosion").second;
		// float eroded = contVal * eroVal;

		// eroded = Game::Generation::Datas::Biomes::GetHeight(biome, eroded);
		// PAVVal *= spl.at("P&V").second;
		return static_cast<uint8_t>(GetBlendedHeightAt(x, z, seed, spl, 4));
	}

    inline uint8_t GetHeightFromCache(int x, int z, const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl, const Generation::Utils::ChunckCache &cache)
    {
        size_t index = (x + Generation::Utils::GENERATION_BLEND_RADIUS) * Generation::Utils::CACHE_SIZE +
               (z + Generation::Utils::GENERATION_BLEND_RADIUS);
		float contVal =
			spl.at("Continental")
				.first.GetValue(cache.continental[index]);

		float eroVal =
			spl.at("Erosion").first.GetValue(cache.erosion[index]);

		// double PAVVal = cache.peaks[index];

		// PAVVal = 1 - std::abs(3 * std::abs(PAVVal) - 2);
		// PAVVal = spl.at("P&V").first.GetValue(PAVVal);
		contVal *= spl.at("Continental").second;

		auto biome = cache.biome[index];

		// eroVal *= spl.at("Erosion").second;
		float eroded = contVal * eroVal;

		eroded = Game::Generation::Datas::Biomes::GetHeight(biome, eroded);
		// PAVVal *= spl.at("P&V").second;
        return static_cast<uint8_t>(eroded);

    }

	inline bool IsBlockAt(const Game::Utils::Defines::Vector3Int pos, const uint32_t &seed,
						  const std::unordered_map<std::string, std::pair<const Spline::Spline, float>> &spl)
	{
		// uint8_t h = GetHeightAt(pos[0], pos[2], seed, spl);
		// return pos[1] <= h;
        (void)pos;
        (void)seed;
        (void)spl;
        return false;
	}
} // namespace Vox::Game::Generation::Perlins

#endif // __PERLININTERPRETATION_HPP__