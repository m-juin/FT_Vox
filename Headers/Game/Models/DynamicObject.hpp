#ifndef __DYNAMICOBJECT_HPP__
#define __DYNAMICOBJECT_HPP__

#include "Game/Scenes/World/Utils/Defines.hpp"

#include "MathGraphicalLib/Vectors/Defines.hpp"

#include "Utils/AUpdatable.hpp"

#include <array>

#include "Front/Rendering/SyncObjects.hpp"

namespace Vox::Game::Models
{
    using namespace Game::Utils::Defines;
    using namespace MGL::Vectors::Types;

    class DynamicObject : public virtual Vox::Utils::AUpdatable
    {
        public:
            DynamicObject(Vector3float defaultPos = {0.0f, 0.0f, 0.0f}, Vector3float defaultRot = {0.0f, 0.0f, 0.0f}, uint32_t updateDelay = 1);
            virtual ~DynamicObject();
    
            virtual bool AssignModel() = 0;
            Matrix GetModel();

            void SetPos(Vector3float newPos);
            void SetRot(Vector3float newRot);
            void Move(Vector3float moveValue);
            void Rotate(Vector3float rotValue);

            Vector3float GetPosition() {return this->_position;}

        protected:
            std::array<bool, MAX_FRAMES_IN_FLIGHT> _isDirty;

            Vector3float _position;
            Vector3float _rotation;
    
    };
}

#endif // __DYNAMICOBJECT_HPP__