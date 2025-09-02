#ifndef __DYNAMICBUFFER_HPP__
#define __DYNAMICBUFFER_HPP__

#include "ABuffer.hpp"

namespace Vox::Front::Rendering::Utils::Buffers
{
    class DynamicBuffer : public ABuffer
    {
        public:
            DynamicBuffer() = delete;
            DynamicBuffer(uint32_t frameCount, VkDeviceSize size, VkBufferUsageFlags usage);
            ~DynamicBuffer();
    
            void Create(void *initialData) override;
            void Update(void *newData, VkDeviceSize newDataSize) override;
            void UpdateAtOffset(size_t frameIndex, size_t offset, void *newData, VkDeviceSize dataSize);

        private:
            std::vector<void *> _mappedMemories;       
            /* private */
    
    };
}

#endif // __DYNAMICBUFFER_HPP__