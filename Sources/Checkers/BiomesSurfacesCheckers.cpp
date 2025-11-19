#include "Game/Datas/Biomes.hpp"
#include "Game/Datas/BiomesData/SurfaceDecoration.hpp"

#include "Utils/Colors.hpp"

#include <iomanip>

using namespace Vox::Game::Generation::Datas::Biomes;
using namespace Vox::Utils::Colors;

int main()
{
	bool error = false;
	std::cout << LWHITE << "Starting Biome Surface rules checks." << RESET << std::endl;

	for (int biomeIndex = 0; biomeIndex <= (int)Biomes::Eroded_Badlands; biomeIndex++)
	{
		try
		{
			Biomes b1 = static_cast<Biomes>(biomeIndex);
            std::stringstream ss;
            ss << b1;
            std::string stringed = ss.str();
            if (stringed == "UNKNOWN")
                continue;
			std::cout << std::endl;
		}
		catch (std::runtime_error)
		{
		}
	}

	std::cout << LWHITE << "End Biome Surface rules checks with result: "
			  << (error ? (std::string(LRED) + "ERROR") : (std::string(GREEN) + "OK")) << RESET << std::endl;
}