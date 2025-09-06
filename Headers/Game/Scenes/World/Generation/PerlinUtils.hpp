#ifndef __PERLINUTILS_HPP__
#define __PERLINUTILS_HPP__

#include <cstdint>

#include "MathGraphicalLib/Vectors/Vector2.hpp"

namespace Vox::World::Generation::Perlins::Utils
{
    struct PerlinData
    {
        float amp;
        float freq;
        uint8_t octaves;
        MGL::Vectors::Vector2<float> offset;
        uint8_t splitted;

        constexpr PerlinData(float amp_, float freq_, uint8_t octaves_, MGL::Vectors::Vector2<float> offset_, uint8_t splitted_) : amp(amp_), freq(freq_), octaves(octaves_), offset(offset_), splitted(splitted_) {}
    };
    constexpr PerlinData ContinentalnessData(1.2f, 0.001f, 8, MGL::Vectors::Vector2<float>(0.0f, 0.0f), 6);

} // namespace Vox::World::Generation::Perlins::Utils

#endif // __PERLINUTILS_HPP__