#ifndef __BIOMESPERLIN_HPP__
#define __BIOMESPERLIN_HPP__

#include "./Perlin.hpp"
#include "Game/Utils/Datas/Biomes.hpp"

namespace Vox::Game::Generation::Perlins
{
	using namespace Datas::Biomes;

	static float GetErosionValueAt(float x, float y, uint32_t seed)
	{
		float val = GetPerlinValue(x, y, seed, Utils::ErosionData, {-1, 1});

		if (val <= -0.78)
			val = 0.0f;
		else if (val <= -0.375)
			val = 1.0f;
		else if (val <= -0.2225)
			val = 2.0f;
		else if (val <= 0.05)
			val = 3.0f;
		else if (val <= 0.45)
			val = 4.0;
		else if (val <= 0.55)
			val = 5.0f;
		else
			val = 6.0f;
		return val;
	}

	static float GetContinentalValueAt(float x, float y, uint32_t seed)
	{
		float val = GetPerlinValue(x, y, seed, Utils::ContinentalnessData, {-1.2f, 1.0f});
		if (val <= -1.05)
			val = 0.0f;
		else if (val <= -0.455)
			val = 1.0f;
		else if (val <= -0.19)
			val = 2.0f;
		else if (val <= -0.11)
			val = 3.0f;
		else if (val <= 0.03)
			val = 4.0f;
		else if (val <= 0.3)
			val = 5.0f;
		else
			val = 6.0f;
		return val;
	}

	static float GetHumidityValueAt(float x, float y, uint32_t seed)
	{
		float val = GetPerlinValue(x, y, seed, Utils::HumidityData, {-1, 1});

		if (val <= -0.35f)
			val = 0.0f;
		else if (val <= -0.1f)
			val = 1.0f;
		else if (val <= 0.1f)
			val = 2.0f;
		else if (val <= 0.3f)
			val = 3.0f;
		else
			val = 4.0f;
		return val;
	}

	static float GetPeaksAndValleyValueAt(float x, float y, uint32_t seed)
	{
		float val = GetPerlinValue(x, y, seed, Utils::PeaksAndValleyData, {-1, 1});

		val = 1 - std::abs(3 * std::abs(val) - 2);
		if (val <= -0.85)
			val = 0.0f;
		else if (val <= -0.6f)
			val = 1.0f;
		else if (val <= 0.2f)
			val = 2.0f;
		else if (val <= 0.7f)
			val = 3.0f;
		else
			val = 4.0f;
		return val;
	}

	static float GetTemperatureValueAt(float x, float y, uint32_t seed)
	{
		float val = GetPerlinValue(x, y, seed, Utils::TemperatureData, {-1, 1});

		if (val <= -0.45f)
			val = 0.0f;
		else if (val <= -0.15f)
			val = 1.0f;
		else if (val <= 0.2f)
			val = 2.0f;
		else if (val <= 0.55f)
			val = 3.0f;
		else
			val = 4.0f;
		return val;
	}

	static float GetWeirdnessValueAt(float x, float y, uint32_t seed)
	{
		float val = GetPerlinValue(x, y, seed, Utils::WeirdnessData, {-1, 1});

		return val;
	}

	inline Biomes GetMiddleBiome(float xPos, float yPos, float tempVal, uint32_t seed)
	{
		if (tempVal == 4.0f)
			return Biomes::Desert;
		float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
		float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
		if (tempVal == 0.0f)
		{
			if (HumidityVal == 0.0f && WeirdnessVal > 0.0f)
				return Biomes::Ice_Spikes;
			if (HumidityVal == 2.0f && WeirdnessVal > 0.0f)
				return Biomes::Snowy_Taiga;
			if (HumidityVal <= 2.0f)
				return Biomes::Snowy_Plains;
			return HumidityVal == 3.0f ? Biomes::Snowy_Taiga : Biomes::Taiga;
		}
		if (tempVal == 1.0f)
		{
			if (HumidityVal <= 1.0f)
				return Biomes::Plains;
			if (HumidityVal == 2.0f)
				return Biomes::Forest;
			if (HumidityVal == 3.0f)
				return Biomes::Taiga;
			return WeirdnessVal > 0.0f ? Biomes::Old_Growth_Pine_Taiga : Biomes::Old_Growth_Spruce_Taiga;
		}
		if (tempVal == 2.0f)
		{
			if (HumidityVal == 0.0f)
				return WeirdnessVal > 0.0f ? Biomes::Sunflower_Plains : Biomes::Flower_Forest;
			if (HumidityVal == 1.0f)
				return Biomes::Plains;
			if (HumidityVal == 2.0f)
				return Biomes::Forest;
			if (HumidityVal == 3.0f)
				return WeirdnessVal > 0.0f ? Biomes::Old_Growth_Birch_Forest : Biomes::Birch_Forest;
			return Biomes::Dark_Forest;
		}
		if (HumidityVal <= 1.0f)
			return Biomes::Savanna;
		if (HumidityVal == 2.0f)
			return WeirdnessVal > 0.0f ? Biomes::Plains : Biomes::Forest;
		if (HumidityVal == 3.0f)
			return WeirdnessVal > 0.0f ? Biomes::Sparse_Jungle : Biomes::Jungle;
		return WeirdnessVal > 0.0f ? Biomes::Bamboo_Jungle : Biomes::Jungle;
	}

	inline Biomes GetPlateauBiome(float xPos, float yPos, float tempVal, uint32_t seed)
	{
		float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
		float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
		if (tempVal == 0.0f)
		{
			if (HumidityVal == 0.0f && WeirdnessVal > 0.0f)
				return Biomes::Ice_Spikes;
			return HumidityVal <= 2.0f ? Biomes::Snowy_Plains : Biomes::Snowy_Taiga;
		}
		if (tempVal == 1.0f)
		{
			if (HumidityVal == 0.0f)
				return WeirdnessVal > 0.0f ? Biomes::Cherry_Grove : Biomes::Meadow;
			if (HumidityVal == 1.0f)
				return Biomes::Meadow;
			if (HumidityVal == 2.0f)
				return WeirdnessVal > 0.0f ? Biomes::Meadow : Biomes::Forest;
			if (HumidityVal == 3.0f)
				return WeirdnessVal > 0.0f ? Biomes::Meadow : Biomes::Taiga;
			return WeirdnessVal > 0.0f ? Biomes::Old_Growth_Pine_Taiga : Biomes::Old_Growth_Spruce_Taiga;
		}
		if (tempVal == 2.0f)
		{
			if (HumidityVal <= 1.0f)
				return WeirdnessVal > 0.0f ? Biomes::Cherry_Grove : Biomes::Meadow;
			if (HumidityVal == 2.0f)
				return WeirdnessVal > 0.0f ? Biomes::Forest : Biomes::Meadow;
			if (HumidityVal == 3.0f)
				return WeirdnessVal > 0.0f ? Biomes::Birch_Forest : Biomes::Meadow;
			return Biomes::Dark_Forest;
		}
		if (tempVal == 3.0f)
		{
			if (HumidityVal <= 1.0f)
				return Biomes::Savanna_Plateau;
			if (HumidityVal <= 3.0f)
				return Biomes::Forest;
			return Biomes::Jungle;
		}
		if (HumidityVal <= 1.0f)
			return WeirdnessVal > 0.0f ? Biomes::Eroded_Badlands : Biomes::Badlands;
		if (HumidityVal == 2.0f)
			return Biomes::Badlands;
		return Biomes::Wooded_Badlands;
	}

	inline Biomes GetShatteredBiome(float xPos, float yPos, float tempVal, uint32_t seed)
	{
		if (tempVal == 4.0f)
			return Biomes::Desert;
		float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
		if (tempVal <= 1.0f)
			return HumidityVal <= 1.0f	 ? Biomes::Windswept_Gravelly_Hills
				   : HumidityVal == 2.0f ? Biomes::Windswept_Hills
										 : Biomes::Windswept_Forest;
		if (tempVal == 2.0f)
			return HumidityVal <= 2.0f ? Biomes::Windswept_Hills : Biomes::Windswept_Forest;
		if (HumidityVal <= 1.0f)
			return Biomes::Savanna;
		float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
		if (HumidityVal == 2.0f)
			return WeirdnessVal > 0.0f ? Biomes::Plains : Biomes::Forest;
		if (HumidityVal == 3.0f)
			return WeirdnessVal > 0.0f ? Biomes::Sparse_Jungle : Biomes::Jungle;
		return WeirdnessVal > 0.0f ? Biomes::Bamboo_Jungle : Biomes::Jungle;
	}

	inline Biomes GetBeachBiome(float xPos, float yPos, float tempVal)
	{
		(void)xPos;
		(void)yPos;
		return tempVal == 0 ? Biomes::Snowy_Beach : tempVal == 4.0f ? Biomes::Desert : Biomes::Beach;
	}

	inline Biomes GetBadlandBiome(float xPos, float yPos, float tempVal, uint32_t seed)
	{
		(void)tempVal;
		float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
		if (HumidityVal <= 1.0f)
		{
			float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
			return WeirdnessVal > 0.0f ? Biomes::Eroded_Badlands : Biomes::Badlands;
		}
		return HumidityVal == 2.0f ? Biomes::Badlands : Biomes::Wooded_Badlands;
	}

	static Biomes GetBiomeAtPoint(float xPos, float yPos, uint32_t seed)
	{
		float contVal = GetContinentalValueAt(xPos, yPos, seed);
		float tempVal = GetTemperatureValueAt(xPos, yPos, seed);
		if (contVal == 0.0f)
			return Biomes::Mushroom_Fields;
		if (contVal <= 2.0f)
		{
			if (tempVal == 4.0f)
				return Biomes::Warm_Ocean;
			int deep = (contVal == 1.0f) ? 1 : 2;
			return (Biomes)(floor(tempVal / 2.0f) + deep);
		}
		float PeaksVal = GetPeaksAndValleyValueAt(xPos, yPos, seed);
		;
		float erosionVal = GetErosionValueAt(xPos, yPos, seed);
		if (PeaksVal == 0.0f /* Valley */)
		{
			if (contVal == 3.0f)
				return tempVal == 0.0f ? Biomes::Frozen_River : Biomes::River;
			if (erosionVal == 6.0f)
				return tempVal == 0.0f	 ? Biomes::Frozen_River
					   : tempVal <= 2.0f ? Biomes::Swamp
										 : Biomes::Mangrove_Swamp;
			if (contVal > 4.0f && erosionVal <= 1.0f)
				return tempVal < 4.0f ? GetMiddleBiome(xPos, yPos, tempVal, seed)
									  : GetBadlandBiome(xPos, yPos, tempVal, seed);
			return tempVal == 0.0f ? Biomes::Frozen_River : Biomes::River;
		}
		else if (PeaksVal <= 1.0f)
		{
			if (contVal <= 3.0f)
			{
				if (erosionVal <= 2.0f)
					return Biomes::Stony_Shore;
				float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				if (erosionVal == 5.0f && WeirdnessVal > 0.0f)
					return tempVal <= 1.0f || HumidityVal == 4.0f ? GetMiddleBiome(xPos, yPos, tempVal, seed)
																  : Biomes::Windswept_Savanna;
				return GetBeachBiome(xPos, yPos, tempVal);
			}
			if (erosionVal == 6.0f)
				return tempVal == 0		 ? GetMiddleBiome(xPos, yPos, tempVal, seed)
					   : tempVal <= 2.0f ? Biomes::Swamp
										 : Biomes::Mangrove_Swamp;
			if (contVal <= 4.0f)
			{
				if (erosionVal <= 1.0f && tempVal == 4.0f)
					return GetBadlandBiome(xPos, yPos, tempVal, seed);
				float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				if (erosionVal == 5.0f && WeirdnessVal > 0 && tempVal >= 2.0f && HumidityVal <= 3.0f)
					return Biomes::Windswept_Savanna;
				return GetMiddleBiome(xPos, yPos, tempVal, seed);
			}
			if (erosionVal <= 1.0f && tempVal == 0.0f)
			{
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				return HumidityVal <= 1.0f ? Biomes::Snowy_Slopes : Biomes::Grove;
			}
			return erosionVal <= 3.0f && tempVal == 4.0f ? GetBadlandBiome(xPos, yPos, tempVal, seed)
														 : GetMiddleBiome(xPos, yPos, tempVal, seed);
		}
		else if (PeaksVal == 2.0f /* Mid */)
		{
			if (contVal <= 3.0f)
			{
				if (erosionVal <= 2.0f)
					return Biomes::Stony_Shore;
				float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				if (erosionVal == 5.0f && WeirdnessVal > 0.0f && (tempVal >= 2.0f || HumidityVal <= 3.0f))
					return Biomes::Windswept_Savanna;
				return WeirdnessVal <= 0.0f ? GetBeachBiome(xPos, yPos, tempVal)
											: GetMiddleBiome(xPos, yPos, tempVal, seed);
			}
			if (erosionVal == 6.0f)
				return tempVal == 0.0f	 ? GetMiddleBiome(xPos, yPos, tempVal, seed)
					   : tempVal <= 2.0f ? Biomes::Swamp
										 : Biomes::Mangrove_Swamp;
			if (erosionVal <= 0.0f)
			{
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				return tempVal >= 3.0f		? GetPlateauBiome(xPos, yPos, tempVal, seed)
					   : HumidityVal <= 1.0 ? Biomes::Snowy_Slopes
											: Biomes::Grove;
			}
			if (erosionVal == 1.0f)
			{
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				if (tempVal == 0.0f)
					return HumidityVal <= 1.0f ? Biomes::Snowy_Slopes : Biomes::Grove;
				if (contVal == 6.0f)
					return GetPlateauBiome(xPos, yPos, tempVal, seed);
				return tempVal == 4.0f ? GetBadlandBiome(xPos, yPos, tempVal, seed)
									   : GetMiddleBiome(xPos, yPos, tempVal, seed);
			}
			if (contVal <= 6.0f)
			{
				if (erosionVal == 5.0f)
				{
					float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
					float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
					if (WeirdnessVal > 0.0f && tempVal >= 2.0f && HumidityVal <= 3.0f)
						return Biomes::Windswept_Savanna;
				}
				return GetMiddleBiome(xPos, yPos, tempVal, seed);
			}

			if (erosionVal == 5.0f)
				return GetShatteredBiome(xPos, yPos, tempVal, seed);
			if (erosionVal == 4.0f)
				return GetMiddleBiome(xPos, yPos, tempVal, seed);
			return contVal > 5.0f	 ? GetPlateauBiome(xPos, yPos, tempVal, seed)
				   : tempVal == 4.0f ? GetBadlandBiome(xPos, yPos, tempVal, seed)
									 : GetMiddleBiome(xPos, yPos, tempVal, seed);
		}
		else if (PeaksVal == 3.0f)
		{
			if (erosionVal == 6.0f || erosionVal == 4.0f)
				return GetMiddleBiome(xPos, yPos, tempVal, seed);
			if (contVal == 3.0f)
			{
				if (erosionVal == 5.0f)
				{
					float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
					float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
					if (WeirdnessVal > 0.0f && tempVal > 2.0f && HumidityVal <= 3.0f)
						return Biomes::Windswept_Savanna;
				}
				return GetMiddleBiome(xPos, yPos, tempVal, seed);
			}
			if (contVal <= 4.0f)
			{
				if (erosionVal == 5.0f)
				{
					float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
					float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
					if (WeirdnessVal > 0.0f && tempVal > 2.0f && HumidityVal <= 3.0f)
						return Biomes::Windswept_Savanna;
				}
				if (erosionVal >= 2.0f)
					return GetMiddleBiome(xPos, yPos, tempVal, seed);
				if (erosionVal == 1.0f)
				{
					if (tempVal == 0.0f)
					{
						float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
						return HumidityVal <= 1.0f ? Biomes::Snowy_Slopes : Biomes::Grove;
					}
					return tempVal == 4 ? GetBadlandBiome(xPos, yPos, tempVal, seed)
										: GetMiddleBiome(xPos, yPos, tempVal, seed);
				}
				if (tempVal >= 3.0f)
					return GetPlateauBiome(xPos, yPos, tempVal, seed);
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				return HumidityVal <= 1.0f ? Biomes::Snowy_Slopes : Biomes::Grove;
			}
			if (erosionVal == 0.0f)
			{
				if (tempVal == 4.0f)
					return GetBadlandBiome(xPos, yPos, tempVal, seed);
				else if (tempVal == 3.0f)
					return Biomes::Stony_Peaks;
				float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
				return WeirdnessVal > 0.0f ? Biomes::Frozen_Peaks : Biomes::Jagged_Peaks;
			}
			if (erosionVal == 1.0f)
			{
				if (tempVal >= 3.0f)
					return GetPlateauBiome(xPos, yPos, tempVal, seed);
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				return HumidityVal <= 1.0f ? Biomes::Snowy_Slopes : Biomes::Grove;
			}
			if (erosionVal == 5.0f)
				return GetShatteredBiome(xPos, yPos, tempVal, seed);

			if (erosionVal == 3.0f && contVal <= 5.0f)
				return tempVal == 4.0f ? GetBadlandBiome(xPos, yPos, tempVal, seed)
									   : GetMiddleBiome(xPos, yPos, tempVal, seed);
			return GetPlateauBiome(xPos, yPos, tempVal, seed);
		}
		else
		{
			if (erosionVal == 6.0f || erosionVal == 4.0f)
				return GetMiddleBiome(xPos, yPos, tempVal, seed);
			if (erosionVal == 5.0f)
			{
				float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				return WeirdnessVal > 0.0f and tempVal >= 2.0f and HumidityVal <= 3.0f
						   ? Biomes::Windswept_Savanna
						   : GetShatteredBiome(xPos, yPos, tempVal, seed);
			}
			if (erosionVal == 2.0f)
				return contVal <= 4.0f ? GetMiddleBiome(xPos, yPos, tempVal, seed)
									   : GetPlateauBiome(xPos, yPos, tempVal, seed);
			if (erosionVal == 3.0f)
			{
				if (contVal == 6.0f)
					return GetPlateauBiome(xPos, yPos, tempVal, seed);
				return contVal == 5.0f && tempVal == 4.0f ? GetBadlandBiome(xPos, yPos, tempVal, seed)
														  : GetMiddleBiome(xPos, yPos, tempVal, seed);
			}
			if (tempVal == 4.0f)
				return GetBadlandBiome(xPos, yPos, tempVal, seed);
			if (contVal <= 4.0f && erosionVal == 1.0f)
			{
				if (tempVal > 0.0f)
					return GetMiddleBiome(xPos, yPos, tempVal, seed);
				float HumidityVal = GetHumidityValueAt(xPos, yPos, seed);
				return HumidityVal <= 1.0f ? Biomes::Snowy_Slopes : Biomes::Grove;
			}
			if (tempVal == 3.0f)
				return Biomes::Stony_Peaks;
			float WeirdnessVal = GetWeirdnessValueAt(xPos, yPos, seed);
			return WeirdnessVal > 0.0f ? Biomes::Frozen_Peaks : Biomes::Jagged_Peaks;
		}
	}

	inline std::vector<uint8_t> GenerateBiomeImage(MGL::Vectors::Vector2<int> center, uint32_t seed, uint16_t imgSize,
												   float scale)
	{
		// std::cout << "scale = " << scale << std::endl;
		std::vector<uint8_t> dataArray;
		dataArray.resize(imgSize * imgSize * 4);
		uint16_t halfSize = imgSize / 2;

		MGL::Vectors::Vector2<int> effectivePos = center;
		std::cout << center << std::endl;
		for (int x = -halfSize; x < halfSize; x++)
		{
			effectivePos[0] = center[0] + (x * scale);
			effectivePos[0] += 125000;
			for (int y = -halfSize; y < halfSize; y++)
			{
				effectivePos[1] = center[1] + (y * scale);
				effectivePos[1] += 125000;
				Biomes biome = GetBiomeAtPoint(effectivePos[0], effectivePos[1], seed);
				MGL::Vectors::Vector3<int> color = biomesColors[biome];

				uint64_t index = ((imgSize * (y + halfSize)) + (x + halfSize)) * 4;
				dataArray[index] = color[0];
				dataArray[index + 1] = color[1];
				dataArray[index + 2] = color[2];
				dataArray[index + 3] = 255;
			}
		}
		return dataArray;
	}

	inline void GenerateBiomeImage(std::vector<uint8_t> &target, MGL::Vectors::Vector2<int> center, uint32_t seed, uint16_t imgSize,
												   float scale)
	{
		// std::cout << "scale = " << scale << std::endl;
		uint16_t halfSize = imgSize / 2;

		MGL::Vectors::Vector2<int> effectivePos = center;
		std::cout << center << std::endl;
		for (int x = -halfSize; x < halfSize; x++)
		{
			effectivePos[0] = center[0] + (x * scale);
			effectivePos[0] += 125000;
			for (int y = -halfSize; y < halfSize; y++)
			{
				effectivePos[1] = center[1] + (y * scale);
				effectivePos[1] += 125000;
				Biomes biome = GetBiomeAtPoint(effectivePos[0], effectivePos[1], seed);
				MGL::Vectors::Vector3<int> color = biomesColors[biome];

				uint64_t index = ((imgSize * (y + halfSize)) + (x + halfSize)) * 4;
				target[index] = color[0];
				target[index + 1] = color[1];
				target[index + 2] = color[2];
				target[index + 3] = 255;
			}
		}
		return ;
	}
} // namespace Vox::Game::Generation::Perlins

#endif // __BIOMESPERLIN_HPP__