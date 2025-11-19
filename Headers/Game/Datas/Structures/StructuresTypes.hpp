#ifndef __STRUCTURESTYPES_HPP__
#define __STRUCTURESTYPES_HPP__

#include <iostream>
#include <map>

namespace Vox::Game::Datas::Structures
{
	enum class StructuresType
	{
        None = -1,
		Oak_Tree1 = 0,
		Oak_Tree2 = 1,
	};

	static const std::map<StructuresType, std::string_view> EnumToString = {
		{StructuresType::Oak_Tree1, "Oak_Tree1"},	 {StructuresType::Oak_Tree2, "Oak_Tree2"}
    };

	static const std::map<std::string_view, StructuresType> StringToEnum = []()
	{
		std::map<std::string_view, StructuresType> map;
		for (const auto &[key, value] : EnumToString)
		{
			map[value] = key;
		}
		return map;
	}();
} // namespace Vox::Game::Datas::Structures

#endif // __STRUCTURESTYPES_HPP__