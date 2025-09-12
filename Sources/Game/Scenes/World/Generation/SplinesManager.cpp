#include "Game/Scenes/World/Generation/SplinesManager.hpp"
#include "Utils/RessourcesPackDefines.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

#include <filesystem>

#include <iostream>

namespace Vox::Game::Generation
{
	SplinesManager::SplinesManager(const std::string &splinePath)
	{
		this->LoadSplines(splinePath);
	}

	void SplinesManager::LoadSplines(const std::string &splinePath)
	{
		std::filesystem::path splineFolder(splinePath);
		splineFolder /= Vox::Utils::RessourcesPackDefines::SPLINE_ASSETS_PATH;
		std::unordered_map<std::string, std::pair<std::string, float>> SplinesToLoad{
			{"Continental", {"continental.spline", 0.6}},
			{"Erosion", {"erosion.spline", 0.25}},
			{"P&V", {"PeaksAndValleys.spline", 0.15}},
		};

		for (auto &pair : SplinesToLoad)
		{
			try
			{
				auto path = std::filesystem::path(splineFolder / pair.second.first);
				this->_splines[pair.first] = {Spline::LoadSpline(path.string().c_str()), pair.second.second};
			}
			catch (std::exception &e)
			{
				throw std::runtime_error("[ERROR] Spline not loaded.");
			}
		}
	}

	std::vector<std::string> SplinesManager::GetSplinesNames()
	{
		std::vector<std::string> names;
		names.reserve(this->_splines.size());
		for (auto pair : this->_splines)
			names.emplace_back(pair.first);
		return names;
	}
	
	std::unordered_map<std::string, float> SplinesManager::GetSplinesWeight() const
	{
		std::unordered_map<std::string, float> retVal;
		for (auto pair : this->_splines)
		{
			retVal[pair.first] = pair.second.second;
		}
		return retVal;
	}

	std::unordered_map<std::string, std::pair<const Spline::Spline, float>> SplinesManager::GetSplinesCopy() const
	{
		std::unordered_map<std::string, std::pair<const Spline::Spline, float>> retVal;
		retVal.reserve(_splines.size());
		for (const auto &pair : _splines)
			retVal.emplace(pair.first, std::pair<const Spline::Spline, float>(*pair.second.first, pair.second.second));
		return retVal;
	}

	void SplinesManager::SetNewWeight(std::unordered_map<std::string, float> weightMap)
	{
		for (auto pair : weightMap)
		{
			if (this->_splines[pair.first].second != pair.second)
			{
				this->_splines[pair.first].second = pair.second;
			}
		}
		Game::World::WorldManager::GetInstance().GetGenerationManager().FlagPool();
	}
} // namespace Vox::Game::Generation