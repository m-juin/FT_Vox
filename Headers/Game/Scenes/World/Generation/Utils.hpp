#ifndef __GENUTILS_HPP__
#define __GENUTILS_HPP__

#include <iostream>

#include <random>   

namespace Vox::Game::Generation::Utils
{
    inline const std::string GenerateSeed()
    {
        return std::to_string(rand());
    }
} // namespace Vox::Game::Generation::Utils


#endif // __GENUTILS_HPP__