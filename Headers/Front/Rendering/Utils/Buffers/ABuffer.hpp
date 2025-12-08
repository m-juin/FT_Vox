#ifndef __ABUFFER_HPP__
#define __ABUFFER_HPP__

#include <vulkan/vulkan.h>
#include <vector>

namespace Vox::Front::Rendering::Utils::Buffers
{
	class ABuffer
	{
		public:
			ABuffer(uint32_t frameCount) : _buffers(frameCount), _memories(frameCount) {};
			virtual ~ABuffer()
            {
                
            };

            virtual void Create(void *initialData) = 0;
            virtual void Update(void *newData, VkDeviceSize newDataSize) = 0;
            virtual void CleanUp() = 0;

            VkBuffer &GetBuffer(size_t frame) {return _buffers[frame];};

		protected:
            std::vector<VkBuffer> _buffers;
            std::vector<VkDeviceMemory> _memories;

            VkDeviceSize _size;
            VkBufferUsageFlags _usage;
            VkMemoryPropertyFlags _memoryProprety;
			/* private */
	};
} // namespace Vox::Front::Rendering::Utils::Buffers

#endif // __ABUFFER_HPP__