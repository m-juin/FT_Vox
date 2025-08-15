#ifndef __STATICBUFFER_HPP__
#define __STATICBUFFER_HPP__

#include "ABuffer.hpp"

namespace Vox::Front::Rendering::Utils::Buffers
{
    class StaticBuffer : public ABuffer
    {
        public:
            StaticBuffer(uint32_t frameCount, VkDeviceSize size, VkBufferUsageFlags usage);
            ~StaticBuffer();
    
            void Create(void *initialData) override;
            void Update(void *newData, VkDeviceSize newDataSize) override;

        private:
            /* private */
    
    };
}

#endif // __STATICBUFFER_HPP__