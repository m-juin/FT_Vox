#ifndef __E_INPUTMASK_HPP__
#define __E_INPUTMASK_HPP__

#include <cstdint>

namespace Vox::Game::Utils::Datas
{
    namespace InputMask
    {
        constexpr uint8_t Mouse = 1 << 7;
        constexpr uint8_t KeyBoard = 1 << 6;
    }
}

#endif // __E_INPUTMASK_HPP__