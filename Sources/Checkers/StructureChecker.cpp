#include "Game/Datas/Structures/Structure.hpp"
#include "Game/Datas/Structures/StructuresTypes.hpp"

#include "Utils/Colors.hpp"

#include <iomanip>
#include <iostream>
#include <unordered_map>

using namespace Vox::Utils::Colors;
using namespace Vox::Game::Datas::Structures;

static const std::unordered_map<StructuresType, std::string> paths{
	{StructuresType::Oak_Tree1, "Assets/TexturesPacks/1/Structures/Trees/Oak1.st"},
	{StructuresType::Oak_Tree1, "Assets/TexturesPacks/1/Structures/Trees/Oak2.st"},
	{StructuresType::Oak_Tree1, "Assets/TexturesPacks/1/Structures/Trees/Oak3.st"},
};

int main()
{
	bool error = false;
	std::cout << LWHITE << "Starting structures checks." << RESET << std::endl;

	for (const auto &[structureType, path] : paths)
	{
		std::cout << MAGENTA << std::left << std::setw(30) << static_cast<int>(structureType) << LWHITE << std::setw(5)
				  << ":";
		try
		{
			Structure st(path);
			std::cout << GREEN << "OK";
		}
		catch (std::runtime_error &e)
		{
			std::cout << LRED << "ERROR : " << e.what() ;
			error = true;
		}
		std::cout << std::endl;
	}

	std::cout << LWHITE << "End structures checks with result: "
			  << (error ? (std::string(LRED) + "ERROR") : (std::string(GREEN) + "OK")) << RESET << std::endl;
}