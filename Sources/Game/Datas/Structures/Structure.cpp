#include "Game/Datas/Structures/Structure.hpp"

#include <iostream>
#include <fstream>

namespace Vox::Game::Datas::Structures
{
    Structure::Structure(const std::string &path)
    {
        std::ifstream st(path);
        if (st.is_open() == false)
            throw std::runtime_error("Failed to load a default struct file.");
        std::string line;
        while (std::getline(st, line))
        {
            std::cout << "[DEBUG] " << line << std::endl;
        }
        
    }
} // namespace Vox::Game::Datas::Structures
