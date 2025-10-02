#ifndef __DECORATIONRULE_HPP__
#define __DECORATIONRULE_HPP__

#include <cstdint>
#include <iostream>

namespace Vox::Game::Generation::Datas::Biomes
{
    struct SurfaceRule
    {
        const uint16_t surfaceDist;
        const std::string blockType;
    };
} // namespace Vos::Game::Utils::Generation


#endif // __DECORATIONRULE_HPP__