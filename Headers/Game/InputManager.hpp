#ifndef __INPUTMANAGER_HPP__
#define __INPUTMANAGER_HPP__

#include "./Utils/Datas/E_InputAction.hpp"
#include "Utils/AUpdatable.hpp"


namespace Vox::Game
{
    class GameManager;
    using namespace Utils::Datas;
    enum class E_InputTarget
    {
        UI,
        Camera
    };
    class InputManager : public Vox::Utils::AUpdatable
    {
        friend class GameManager;
        public:
            InputManager();
            ~InputManager();
        
            void SetInputTarget(E_InputTarget newTarget);
            E_InputTarget GetInputTarget() { return this->_target; };
        
        private:
            std::unordered_map<E_InputAction, int> _inputMap;

            E_InputTarget _target;
            void HandlePerFrameInput();
            void SetupCallBack();
            void LoadInput();
            /* private */
    };
    
} // namespace Vox::Game

#endif // __INPUTMANAGER_HPP__