#ifndef __SAVESDATA_HPP__
#define __SAVESDATA_HPP__

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

#include <unordered_map>

namespace Vox::Game::Scenes::Menu::Saves
{
	constexpr const char *WorldsFolder = "./WorldsData";

	inline constexpr const char *saveKeys[3] = {
		"FolderPath",
		"SaveName",
		"seed",
	};

	struct WorldData
	{
			std::string folderPath;
			std::string worldName;
			std::string seed;

			WorldData(const std::string &folderPath_, const std::string &worldName_, const std::string &seed_)
				: folderPath(folderPath_), worldName(worldName_), seed(seed_)
			{
			}
			friend std::ostream &operator<<(std::ostream &os, const WorldData &wd);
	};

	inline std::ostream &operator<<(std::ostream &os, const WorldData &wd)
	{
		os << "FolderPath = " << wd.folderPath << "\nWorldName = " << wd.worldName << "\nSeed = " << wd.seed
		   << std::endl;
		return os;
	}

	inline std::pair<std::string, std::string> LineParser(const std::string &ligne)
	{
		size_t sepPos = ligne.find(':');

		if (sepPos == std::string::npos)
			throw std::invalid_argument("Bad Format");

		std::string key = ligne.substr(0, sepPos);
		key.erase(0, key.find_first_not_of(" \'\","));
		key.erase(key.find_last_not_of(" \'\",") + 1);

		std::string val = ligne.substr(sepPos + 1);
		val.erase(0, val.find_first_not_of(" \'\","));
		val.erase(val.find_last_not_of(" \'\",") + 1);

		return std::make_pair(key, val);
	}

	inline WorldData LoadWorldData(const std::string &path)
	{
		std::fstream fs(path);

		if (fs.is_open() == false)
			return WorldData("", "", "");

		std::unordered_map<std::string, std::string> map;
		std::string line;
		const std::regex patternAcco("[{}]");

		while (std::getline(fs, line))
		{
			if (std::regex_search(line, patternAcco) == true)
				continue;

			try
			{
				auto parsed = LineParser(line);
				map[parsed.first] = parsed.second;
			}
			catch (const std::runtime_error &e)
			{
				continue;
			}
		}
		for (auto key : saveKeys)
		{
			if (map.find(key) == map.end() || map[key] == "")
			{
				return WorldData("", "", "");
			}
		}

		return WorldData(map["FolderPath"], map["SaveName"], map["seed"]);
	}

	inline void DeleteWorld(WorldData dt)
	{
		const std::filesystem::path path = Saves::WorldsFolder;
		std::filesystem::path saveFile(path / dt.folderPath);
		if (std::filesystem::exists(saveFile / "SaveData.json"))
			std::filesystem::remove_all(saveFile);
	}
} // namespace Vox::Game::Scenes::Menu::Saves

#endif // __SAVESDATA_HPP__