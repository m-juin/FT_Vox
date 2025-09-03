#ifndef __E_BUILDSTATE_HPP__
#define __E_BUILDSTATE_HPP__

namespace Vox::Game::Generation
{
    enum class E_GenerationState
    {
        WaitingThread = 0,
        Mesh = 1,
        WaitingBuffer = 2,
        End = 3
    };
} // namespace Vox::Game::Generation


#endif // __E_BUILDSTATE_HPP__