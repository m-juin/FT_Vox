#ifndef __SAVESDATA_HPP__
#define __SAVESDATA_HPP__

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

#include <unordered_map>

#include "Utils/Strings.hpp"

#include "Game/Generation/Utils.hpp"

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
			WorldData(std::string folderPath_, std::string &worldName_, std::string seed_)
				: folderPath(folderPath_), worldName(worldName_), seed(seed_)
			{
			}
			friend std::ostream &operator<<(std::ostream &os, const WorldData &wd);

			friend std::ostream &ToJSON(std::ostream &os, const WorldData &wd);
	};

	inline std::ostream &ToJSON(std::ostream &os, const WorldData &wd)
	{
		os << "{\n\t\"FolderPath\": \"" << wd.folderPath << "\",\n\t\"SaveName\": \"" << wd.worldName
		   << "\",\n\t\"seed\": \"" << wd.seed << "\"\n}";
		return os;
	}

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
		key.erase(0, key.find_first_not_of(" \'\"\t,"));
		key.erase(key.find_last_not_of(" \'\"\t,") + 1);

		std::string val = ligne.substr(sepPos + 1);
		val.erase(0, val.find_first_not_of(" \'\"\t,"));
		val.erase(val.find_last_not_of(" \'\"\t,") + 1);

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
				std::cout << e.what() << std::endl;
				continue;
			}
		}
		for (auto key : saveKeys)
		{
			if (map.find(key) == map.end() || map[key] == "")
			{
				return WorldData("", "", "");
			}

			std::cout << key << " | " << map[key] << std::endl;
		}

		return WorldData(map["FolderPath"], map["SaveName"], map["seed"]);
	}

	inline void DeleteWorld(WorldData dt)
	{
		const std::filesystem::path path = Saves::WorldsFolder;
		if (std::filesystem::exists(path) == false)
			return;
		std::filesystem::path saveFile(path / dt.folderPath);
		std::cout << dt.folderPath << std::endl;
		if (std::filesystem::exists(saveFile / "SaveData.json"))
			std::filesystem::remove_all(saveFile);
	}

	inline WorldData CreateSaveData(const std::string &worldName, const std::string &seed)
	{
		const std::filesystem::path path = Saves::WorldsFolder;
		WorldData dt{"", worldName, seed};
		dt.folderPath = dt.worldName;
		if (dt.seed.size() != 0 && Vox::Utils::Strings::IsDigit(dt.seed) == false)
			return {"", "", ""};
		else if (dt.seed.size() == 0)
			dt.seed = Vox::Game::Generation::Utils::GenerateSeed();
		return dt;
	}

	inline bool SaveWorldData(const WorldData &dt)
	{
		const std::filesystem::path path = Saves::WorldsFolder;
		if (std::filesystem::exists(path) == false)
			std::filesystem::create_directories(path);
		std::filesystem::path folderPath(path / dt.folderPath);
		if (std::filesystem::exists(folderPath) == true)
			return false;

		std::filesystem::create_directories(folderPath);
		std::filesystem::path fPath(folderPath / "SaveData.json");

		std::ofstream f(fPath);
		if (f.is_open() == false)
			return false;
		ToJSON(f, dt) << std::endl;
		f.close();
		return true;
	}

} // namespace Vox::Game::Scenes::Menu::Saves

#endif // __SAVESDATA_HPP__