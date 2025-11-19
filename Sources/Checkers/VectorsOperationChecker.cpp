#include "Game/Scenes/World/Chuncks/Operations.hpp"

#include "Utils/Colors.hpp"

#include <iomanip>
#include <iostream>

using namespace Vox::Utils::Colors;
using namespace Vox::Game::Chuncks::Operations;
using namespace MGL::Vectors;

struct Answer
{
		ChunkCoord lc;
		WorldCoord wc;
		ClusterCoord cc;
};

std::ostream &operator<<(std::ostream &os, const Answer &a)
{
	os << MAGENTA << a.lc << LWHITE << " | " << MAGENTA << a.wc << LWHITE << " | " << MAGENTA << a.cc << RESET;
	return os;
};

static const std::vector<Answer> tests{{{0, 0, 0}, {0, 0, 0}, {0, 0}},
									   {{15, 0, 0}, {15, 0, 0}, {0, 0}},
									   {{0, 0, 0}, {16, 0, 0}, {1, 0}},
									   {{1, 0, 0}, {-15, 0, 0}, {-1, 0}},
									   {{1, 0, 0}, {175, 81, 321}, {-1, 0}}};

int main()
{
	bool error = false;
	std::cout << LWHITE << "Starting Chunk vectors operations checks." << RESET << std::endl;

	for (size_t i = 0; i < tests.size(); i++)
	{
		std::cout << MAGENTA << std::left << std::setw(30) << i << LWHITE << std::setw(5) << ":";
		try
		{
			const Answer &val = tests[i];
			Answer ans{WorldToChunk(val.wc), ChunkToWorld(val.lc, {val.cc[0], 0, val.cc[1]}), WorldToCluster(val.wc)};
			std::cout << YELLOW << "DEFAULT : \n\t" << val << "\n\t" << ans << std::endl;
		}
		catch (std::runtime_error &e)
		{
			std::cout << LRED << "ERROR : " << e.what();
			error = true;
		}
		std::cout << std::endl;
	}

	std::cout << LWHITE << "End Chunk vectors operations with result: "
			  << (error ? (std::string(LRED) + "ERROR") : (std::string(GREEN) + "OK")) << RESET << std::endl;
}