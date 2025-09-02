#ifndef __AUPDATABLE_HPP__
#define __AUPDATABLE_HPP__

#include <cstdint>

#include "./CallBacksManager.hpp"

namespace Vox::Utils
{
    class AUpdatable
    {
        public:
            AUpdatable(uint32_t delay = 1) : _internalFrameCount(0), _delay(delay < 1 ? 1 : delay) {};
            virtual ~AUpdatable() {};

            inline virtual void Update()
            {
                _internalFrameCount += 1;
                if (_internalFrameCount < _delay)
                    return ;
                _internalFrameCount = 0;
                onUpdate.Notify();
            }

        private:
            uint32_t _internalFrameCount;
            uint32_t _delay;
        protected:
            CallBacksManager<> onUpdate;
            /* private */
    
    };
} // namespace Vox::Utils


#endif // __AUPDATABLE_HPP__