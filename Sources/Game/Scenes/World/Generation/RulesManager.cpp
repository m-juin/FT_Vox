#include "Game/Scenes/World/Generation/RulesManager.hpp"

#include "Game/GameManager.hpp"
#include "JsonLib/Json.hpp"

#include "Game/Datas/Biomes.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{
	std::vector<TreeRule> BuildTreeRules(JSONLib::JSONObject &involved)
	{
		std::cout << "here\n";
		std::vector<TreeRule> ret;
		for (auto item : involved["TreesData"]["TreeTypes"].As<JSONLib::JSONVector>())
		{
			auto dict = item.As<JSONLib::JSONObject>();

			auto it = Game::Datas::Structures::StringToEnum.find(dict["Type"].As<std::string>());
			if (it == Game::Datas::Structures::StringToEnum.end())
				continue;

			TreeRule rule(it->second, dict["Percent"].As<int>());
			ret.push_back(rule);
		}
		return ret;
	}

	std::vector<SurfaceRule> BuildSurfaceRules(JSONLib::JSONObject &involved)
	{
		std::vector<SurfaceRule> ret;
		for (auto item : involved["SurfaceRules"].As<JSONLib::JSONVector>())
			ret.push_back({static_cast<uint16_t>(item["SurfaceDistance"].As<int>()),
						   static_cast<Game::Datas::Blocks::BlockType>(item["Block"].As<int>())});
		return ret;
	}

	RulesManager::RulesManager()
	{
		std::string path = Game::GameManager::GetInstance().GetTexturePackPath() + "Generation/BiomesRules.json";
		JSONLib::Dictionnary rules = JSONLib::LoadJSON(path);
		for (const auto &pair : rules.As<JSONLib::JSONObject>())
		{
			Biomes bi = biomesStringToEnum.at(pair.first);
			JSONLib::JSONObject current = pair.second.As<JSONLib::JSONObject>();
			std::pair<const Biomes, const SurfaceDecoration> paired = {
				bi,
				{BuildSurfaceRules(current),
				 HeightRule(current["HeightRule"]["Multipliyer"].As<double>(),
							static_cast<double>(current["HeightRule"]["MinHeight"].As<int>())),
				 current["WaterAffecter"].As<bool>(), current["TreesData"]["TreeChance"].As<int>(),
				 BuildTreeRules(current)}};

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
	Game::Datas::Structures::StructuresType RulesManager::GetTreeType(Biomes biome, int randVal)
	{
		using Game::Datas::Structures::StructuresType;
		auto &biomesSurfaces = RulesManager::RulesManager::GetInstance().decorationRule;
		auto it = biomesSurfaces.find(biome);
		if (it == biomesSurfaces.end() || it->second.treesRule.size() == 0)
		{
			return StructuresType::None;
		}
		std::pair<int, int> range(0, 0);
		for (const auto &rule : it->second.treesRule)
		{
			range.second = range.first + rule.percent;
			if (randVal < range.second)
				return rule.type;
			range.first = range.second;
		}

		return it->second.treesRule[0].type;
	}

} // namespace Vox::Game::Generation::Datas::Biomes