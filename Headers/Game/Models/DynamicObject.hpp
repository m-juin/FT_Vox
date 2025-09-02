#ifndef __DYNAMICOBJECT_HPP__
#define __DYNAMICOBJECT_HPP__

#include "Game/Scenes/World/Utils/Defines.hpp"

#include "Utils/AUpdatable.hpp"

namespace Vox::Game::Models
{
    using namespace Game::Utils::Defines;

    class DynamicObject : public virtual Vox::Utils::AUpdatable
    {
        public:
            DynamicObject(Vector3 defaultPos = {0.0f, 0.0f, 0.0f}, Vector3 defaultRot = {0.0f, 0.0f, 0.0f}, uint32_t updateDelay = 1);
            virtual ~DynamicObject();
    
            virtual void AssignModel() = 0;
            Matrix GetModel();

            void SetPos(Vector3 newPos);
            void SetRot(Vector3 newRot);
            void Move(Vector3 moveValue);
            void Rotate(Vector3 rotValue);



        protected:
            bool _isDirty;

            Vector3 _position;
            Vector3 _rotation;
    
    };
}

#endif // __DYNAMICOBJECT_HPP__