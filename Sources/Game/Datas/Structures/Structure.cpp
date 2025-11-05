#include "Game/Datas/Structures/Structure.hpp"

#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>

#include "JsonLib/Utils.hpp"
#include "MathGraphicalLib/Vectors/Vector3.hpp"

namespace Vox::Game::Datas::Structures
{
	enum class step
	{
		Size = 0,
		Mapping = 1,
		Design = 2
	};

	struct parsingResult
	{
			MGL::Vectors::Vector3<size_t> _sizes;
			std::map<std::string, Game::Datas::Blocks::BlockType> _types;
	};

	parsingResult ParseContent(const std::unordered_map<step, std::vector<std::string>> &content)
	{
		MGL::Vectors::Vector3<int> _sizes;

		std::regex pattern("[0-9]+, [0-9]+, [0-9]+$");
		parsingResult st;
		auto involved = content.at(step::Size);
		if (involved.size() != 1)
			throw std::runtime_error("Invalid structure size format.");
		if (std::regex_search(involved[0], pattern) == false)
			throw std::runtime_error("Invalid structure size format.");
		auto splitted = JSONLib::Utils::Split(involved[0], ",\0");
		std::stringstream ss;
		for (size_t i = 0; i < 3; i++)
		{
			ss << JSONLib::Utils::Trim(splitted[i]);
			ss >> st._sizes[i];
			if (st._sizes[i] == 0)
				throw std::runtime_error("Invalid structure size(0).");
			ss.str("");
			ss.clear();
		}
		involved = content.at(step::Mapping);
		pattern = std::regex("^\\d+:\\s*[A-Za-z]+(?:[A-Za-z_]*)?$");
		for (auto line : involved)
		{
			auto trimmed = JSONLib::Utils::Trim(line);
			if (std::regex_match(trimmed, pattern) == false)
			{
				ss << "Invalid Block mapping at line \"" << line << "\".";
				throw std::runtime_error(ss.str());
			}
			splitted.clear();
			splitted = JSONLib::Utils::Split(line, ":\0");
            std::cout << splitted[1] << std::endl;
			std::string key = JSONLib::Utils::Trim(splitted[0]);
			std::string enumVal = JSONLib::Utils::Trim(splitted[1]);
			auto it = Game::Datas::Blocks::StringToEnum.find(enumVal);
			if (it == Game::Datas::Blocks::StringToEnum.end())
			{
				ss << "Invalid Block mapping at line \"" << line << "\". Unknown block type.";
				throw std::runtime_error(ss.str());
			}
			st._types[key] = it->second;
		}
		return st;
	}

	Structure::Structure(const std::string &path)
	{
		std::ifstream st(path);
		if (st.is_open() == false)
			throw std::runtime_error("Failed to load a default struct file.");
		std::string line;
		step _step = step::Size;
		std::unordered_map<step, std::vector<std::string>> _content;
		while (std::getline(st, line))
		{
			if (JSONLib::Utils::Trim(line).size() == 0)
				continue;
			if (line == "========" && _step != step::Design)
				_step = (step)((int)(_step) + 1);
			else
				_content[_step].push_back(JSONLib::Utils::Trim(line));
		}

		if (_content.size() != 3)
			throw std::runtime_error("Invalid structure file format.");

		auto parseResult = ParseContent(_content);
		std::cout << "\nSize = " << parseResult._sizes << std::endl;
        for (auto pair : parseResult._types)
        {
            std::cout << "BlockMapping = " << pair.first << " | " <<  (int)pair.second << std::endl;
        }
	}
} // namespace Vox::Game::Datas::Structures
