#ifndef __E_INPUTMASK_HPP__
#define __E_INPUTMASK_HPP__

#include <cstdint>

namespace Vox::Game::Utils::Datas
{
    enum class E_InputMask : uint8_t
    {
        None = 0,
        Mouse = 1 << 7,
        KeyBoard = 1 << 6
    };

    inline uint8_t operator&(E_InputMask a, E_InputMask b)
    {
        return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
    }

    inline uint8_t operator|(E_InputMask a, E_InputMask b)
    {
        return static_cast<uint8_t>(a) | static_cast<uint8_t>(b);
    }

    inline uint8_t operator~(E_InputMask a)
    {
        return ~static_cast<uint8_t>(a);
    }
}

#endif // __E_INPUTMASK_HPP__