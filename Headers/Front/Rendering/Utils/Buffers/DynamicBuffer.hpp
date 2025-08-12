#ifndef __DYNAMICBUFFER_HPP__
#define __DYNAMICBUFFER_HPP__

#include "ABuffer.hpp"

namespace Vox::Front::Rendering::Utils::Buffers
{
    class DynamicBuffer : public ABuffer
    {
        public:
            DynamicBuffer(uint32_t frameCount, VkDeviceSize size, VkBufferUsageFlags usage);
            ~DynamicBuffer();
    
            void Create() override;
            void Update(void *newData, VkDeviceSize newDataSize) override;

        private:
            std::vector<void *> _mappedMemories;       
            /* private */
    
    };
}

#endif // __DYNAMICBUFFER_HPP__