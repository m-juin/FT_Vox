#ifndef __INPUTMANAGER_HPP__
#define __INPUTMANAGER_HPP__

namespace Vox::Game
{
    enum class E_InputTarget
    {
        UI,
        Camera
    };
    class InputManager
    {
        public:
            InputManager();
            ~InputManager();
        
            void SetInputTarget(E_InputTarget newTarget) { this->_target = newTarget; };
            E_InputTarget GetInputTarget() { return this->_target; };
        
        private:
            E_InputTarget _target;
            void LoadInput();
            /* private */
    };
} // namespace Vox::Game

#endif // __INPUTMANAGER_HPP__