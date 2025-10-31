#include "Game/Scenes/World/Generation/RulesManager.hpp"

#include "Game/GameManager.hpp"
#include "JsonLib/Json.hpp"

#include "Game/Datas/Biomes.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{

	RulesManager::RulesManager()
	{
		std::string path = Game::GameManager::GetInstance().GetTexturePackPath() + "Generation/BiomesRules.json";
		JSONLib::Dictionnary rules = JSONLib::LoadJSON(path);
		// JSONLib::Serializer::WriteDictToFD(std::cout, rules, 0, true);
		for (const auto &pair : rules.As<JSONLib::JSONObject>())
		{
			Biomes bi = biomesStringToEnum.at(pair.first);
			JSONLib::JSONObject current = pair.second.As<JSONLib::JSONObject>();
			std::vector<SurfaceRule> vList;
			for (auto item : current["SurfaceRules"].As<JSONLib::JSONVector>())
				vList.push_back(
					{static_cast<uint16_t>(item["SurfaceDistance"].As<int>()), static_cast<Game::Datas::Blocks::BlockType>(item["Block"].As<int>())});
			// std::cout << "hello : " << std::boolalpha << current["HeightRule"]["MinHeight"].IsType<double>() <<
			// std::endl;
			std::pair<const Biomes, const SurfaceDecoration> paired = {
				bi,
				{vList,
				 HeightRule(current["HeightRule"]["Multipliyer"].As<double>(),
							static_cast<double>(current["HeightRule"]["MinHeight"].As<int>())),
				 current["WaterAffecter"].As<bool>()}};

			this->decorationRule.insert(paired);
		}
	}

	Game::Datas::Blocks::BlockType RulesManager::GetBlockType(Biomes biome, int depth)
	{
        auto &biomesSurfaces = RulesManager::RulesManager::GetInstance().decorationRule;
		if (biomesSurfaces.find(biome) == biomesSurfaces.end())
		{
			// if (depth <= 0)
			// return "Grass";
			// else if (depth <= 3)
			// return "Dirt";
			// else
			return Game::Datas::Blocks::BlockType::Stone;
		}
		for (auto &r : biomesSurfaces.at(biome).surfaceRules)
		{
			if (depth <= r.surfaceDist)
				return r._type;
		}
		return Game::Datas::Blocks::BlockType::Stone;
	}

	float RulesManager::GetHeight(Biomes biome, const float &val)
	{
        auto &biomesSurfaces = RulesManager::RulesManager::GetInstance().decorationRule;
		if (biomesSurfaces.find(biome) == biomesSurfaces.end())
			return val;
		return biomesSurfaces.at(biome).heightRules.Apply(val);
	}
} // namespace Vox::Game::Generation::Datas::Biomes