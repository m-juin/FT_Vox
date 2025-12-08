#ifndef __ALATEUPDATABLE_HPP__
#define __ALATEUPDATABLE_HPP__

#include <cstdint>

#include "./CallBacksManager.hpp"

namespace Vox::Utils
{
    class ALateUpdatable
    {
        public:
            ALateUpdatable(uint32_t delay = 1) : _internalFrameCount(0), _delay(delay < 1 ? 1 : delay) {};
            virtual ~ALateUpdatable() {};

            inline virtual void LateUpdate()
            {
                _internalFrameCount += 1;
                if (_internalFrameCount < _delay)
                    return ;
                _internalFrameCount = 0;
                onLateUpdate.Notify();
            }

        private:
            uint32_t _internalFrameCount;
            uint32_t _delay;
        protected:
            CallBacksManager<> onLateUpdate;
            /* private */
    
    };
} // namespace Vox::Utils


#endif // __ALATEUPDATABLE_HPP__