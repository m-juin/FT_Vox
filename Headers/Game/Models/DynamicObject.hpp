#ifndef __DYNAMICOBJECT_HPP__
#define __DYNAMICOBJECT_HPP__

#include "Game/Scenes/World/Utils/Defines.hpp"

#include "Utils/AUpdatable.hpp"

#include <array>

#include "Front/Rendering/SyncObjects.hpp"

namespace Vox::Game::Models
{
    using namespace Game::Utils::Defines;

    class DynamicObject : public virtual Vox::Utils::AUpdatable
    {
        public:
            DynamicObject(Vector3Float defaultPos = {0.0f, 0.0f, 0.0f}, Vector3Float defaultRot = {0.0f, 0.0f, 0.0f}, uint32_t updateDelay = 1);
            virtual ~DynamicObject();
    
            virtual bool AssignModel() = 0;
            Matrix GetModel();

            void SetPos(Vector3Float newPos);
            void SetRot(Vector3Float newRot);
            void Move(Vector3Float moveValue);
            void Rotate(Vector3Float rotValue);

            Vector3Float GetPosition() {return this->_position;}

        protected:
            std::array<bool, MAX_FRAMES_IN_FLIGHT> _isDirty;

            Vector3Float _position;
            Vector3Float _rotation;
    
    };
}

#endif // __DYNAMICOBJECT_HPP__