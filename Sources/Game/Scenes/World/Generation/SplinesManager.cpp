#include "Game/Scenes/World/Generation/SplinesManager.hpp"
#include "Utils/RessourcesPackDefines.hpp"

#include "Game/GameManager.hpp"

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
		std::unordered_map<std::string, std::string> SplinesToLoad{
			{"Continental", "continental.spline"},
			{"Erosion", "erosion.spline"},
			{"P&V", "PeaksAndValleys.spline"},
		};

		for (auto &pair : SplinesToLoad)
		{
			try
			{
				auto path = std::filesystem::path(splineFolder / pair.second);
				this->_splines[pair.first] =
					Spline::LoadSpline(path.string().c_str());
			}
			catch (std::exception &e)
			{
				throw std::runtime_error("[ERROR] Spline not loaded.");
			}
		}
	}

	std::unordered_map<std::string, const Spline::Spline> SplinesManager::GetSplinesCopy() const
	{
		std::unordered_map<std::string, const Spline::Spline> retVal;
		retVal.reserve(_splines.size());
		for (const auto &pair : _splines)
			retVal.emplace(pair.first, *pair.second);
		return retVal;
	}
} // namespace Vox::Game::Generation