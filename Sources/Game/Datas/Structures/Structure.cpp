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
		Anchor = 1,
		Mapping = 2,
		Design = 3
	};

	struct parsingResult
	{
			MGL::Vectors::Vector3<size_t> _sizes;
			MGL::Vectors::Vector3<size_t> _anchor;
			std::map<std::string, Game::Datas::Blocks::BlockType> _types;
	};

	MGL::Vectors::Vector3<size_t> ParseVectors(std::vector<std::string> fileData,
											   std::function<bool(size_t val)> isValid, std::string invalidMessage)
	{
		std::regex pattern("[0-9]+, [0-9]+, [0-9]+$");
		if (fileData.size() != 1)
			throw std::runtime_error(invalidMessage);
		if (std::regex_search(fileData[0], pattern) == false)
			throw std::runtime_error(invalidMessage);
		auto splitted = JSONLib::Utils::Split(fileData[0], ",\0");
		std::stringstream ss;
		MGL::Vectors::Vector3<size_t> ret;
		for (size_t i = 0; i < 3; i++)
		{
			ss << JSONLib::Utils::Trim(splitted[i]);
			ss >> ret[i];
			if (isValid != nullptr && isValid(ret[i]) == false)
				throw std::runtime_error(invalidMessage);
			ss.str("");
			ss.clear();
		}
		return ret;
	}

	parsingResult ParseDatas(const std::unordered_map<step, std::vector<std::string>> &content)
	{
		parsingResult st;
		st._sizes =
			ParseVectors(content.at(step::Size), [](size_t val) { return val != 0; }, "Invalid structure size format.");
		st._anchor = ParseVectors(content.at(step::Anchor), nullptr, "Invalid structure anchor format.");
		auto involved = content.at(step::Mapping);
		std::regex pattern("^\\d+:\\s*[A-Za-z]+(?:[A-Za-z_]*)?$");
		std::stringstream ss;
		for (auto line : involved)
		{
			auto trimmed = JSONLib::Utils::Trim(line);
			if (std::regex_match(trimmed, pattern) == false)
			{
				ss << "Invalid Block mapping at line \"" << line << "\".";
				throw std::runtime_error(ss.str());
			}
			auto splitted = JSONLib::Utils::Split(line, ":\0");
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
		std::unordered_map<step, std::vector<std::string>> fileDatas;
		while (std::getline(st, line))
		{
			if (JSONLib::Utils::Trim(line).size() == 0)
				continue;
			if (line == "========" && _step != step::Design)
				_step = (step)((int)(_step) + 1);
			else
				fileDatas[_step].push_back(JSONLib::Utils::Trim(line));
		}

		if (fileDatas.size() != (size_t)step::Design + 1)
			throw std::runtime_error("Invalid structure file format.");

		auto parseResult = ParseDatas(fileDatas);
		this->_structureSize = parseResult._sizes;
		this->_anchorPoint = parseResult._anchor;
		this->_content.resize(this->_structureSize[0] * this->_structureSize[1] * this->_structureSize[2]);
		this->BuildContent(fileDatas.at(step::Design), parseResult._types);
	}

	size_t Structure::GetLocalIndex(MGL::Vectors::Vector3<size_t> pos) { return pos[0] + (pos[2] * (this->_structureSize[0]) +
										 (pos[1] * (this->_structureSize[0] * this->_structureSize[2])));} 

	void Structure::BuildContent(std::vector<std::string> fileData,
								 const std::map<std::string, Game::Datas::Blocks::BlockType> &mapping)
	{
		if (fileData.size() != ((this->_structureSize[1] * this->_structureSize[2]) + (this->_structureSize[1] - 1)))
			throw std::runtime_error("Invalid structure file. Error at design part.");
		MGL::Vectors::Vector3<size_t> pos = {0, 0, 0};
		for (const std::string &line : fileData)
		{
			if (line == "========")
			{
				pos[1]++;
				pos[2] = 0;
				continue;
			}
			auto splitted = JSONLib::Utils::Split(line, " ");
			if (splitted.size() != this->_structureSize[0])
			{
				std::stringstream ss;
				ss << "Invalid structure file. Error at design line: \"" << line << "\" Expecting "
				   << (int)this->_structureSize[0] << "values and got " << splitted.size();
				throw std::runtime_error(ss.str());
			}
			pos[0] = 0;
			for (auto val : splitted)
			{
				size_t index = this->GetLocalIndex(pos);
				auto blockType = mapping.find(val);
				if (blockType == mapping.end())
				{
					std::stringstream ss;
					ss << "Invalid structure file. Error at design line: \"" << line << "\" unknown value: "
					   << val;
					throw std::runtime_error(ss.str());
				}
				this->_content[index] = blockType->second;
				pos[0]++;
			}
			pos[2]++;
		}
	}
} // namespace Vox::Game::Datas::Structures
