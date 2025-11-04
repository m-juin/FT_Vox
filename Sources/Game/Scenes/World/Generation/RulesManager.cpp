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
		for (const auto &pair : rules.As<JSONLib::JSONObject>())
		{
			Biomes bi = biomesStringToEnum.at(pair.first);
			JSONLib::JSONObject current = pair.second.As<JSONLib::JSONObject>();
			std::vector<SurfaceRule> vList;
			for (auto item : current["SurfaceRules"].As<JSONLib::JSONVector>())
				vList.push_back(
					{static_cast<uint16_t>(item["SurfaceDistance"].As<int>()), static_cast<Game::Datas::Blocks::BlockType>(item["Block"].As<int>())});
			std::pair<const Biomes, const SurfaceDecoration> paired = {
				bi,
				{vList,
				 HeightRule(current["HeightRule"]["Multipliyer"].As<double>(),
							static_cast<double>(current["HeightRule"]["MinHeight"].As<int>())),
				 current["WaterAffecter"].As<bool>(), current["TreeChance"].As<double>()}};

			this->decorationRule.insert(paired);
		}
	}

	Game::Datas::Blocks::BlockType RulesManager::GetBlockType(Biomes biome, int depth)
	{
        auto &biomesSurfaces = RulesManager::RulesManager::GetInstance().decorationRule;
		if (biomesSurfaces.find(biome) == biomesSurfaces.end())
		{
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

	float RulesManager::GetTreeChance(Biomes biome)
	{
        auto &biomesSurfaces = RulesManager::RulesManager::GetInstance().decorationRule;
		if (biomesSurfaces.find(biome) == biomesSurfaces.end())
			return 4.0;
		return biomesSurfaces.at(biome).treeChance;
	}

} // namespace Vox::Game::Generation::Datas::Biomes