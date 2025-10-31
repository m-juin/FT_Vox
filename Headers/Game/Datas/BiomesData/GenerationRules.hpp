#ifndef __GENERATIONRULES_HPP__
#define __GENERATIONRULES_HPP__

#include <cstdint>
#include <iostream>

#include "../Blocks/BlocksType.hpp"

namespace Vox::Game::Generation::Datas::Biomes
{
    struct SurfaceRule
    {
        const uint16_t surfaceDist;
        const Game::Datas::Blocks::BlockType _type;
    };
    struct HeightRule
    {
        const float heightMultiplayer;
        const float minHeight;   
        float Apply(const float &val) const
        {
            return val * heightMultiplayer + minHeight;
        }
        // HeightRule() : heightMultiplayer(1.0f), minHeight(80) {};
        HeightRule(float heightMultiplayer_ = 1.0f, float minHeight_ = 80) : heightMultiplayer(heightMultiplayer_), minHeight(minHeight_) {};
        
    };
} // namespace Vos::Game::Utils::Generation


#endif // __GENERATIONRULES_HPP__