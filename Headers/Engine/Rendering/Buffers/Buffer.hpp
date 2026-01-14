#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace Vox::Engine::Rendering::Buffers
{
	class Buffer
	{

		public:
#pragma region Constructors
			Buffer(uint32_t frameCount, VkDeviceSize size, VkBufferUsageFlags usage) : _buffers(frameCount), _memories(frameCount), _mappedMemories(frameCount), _size(size), _usage(usage) {};
			Buffer() = delete;
			~Buffer();
#pragma endregion

#pragma region Getters
			VkBuffer &GetBuffer(size_t frame)
			{
				return _buffers[frame];
			};
#pragma endregion

			void CleanUp();

			void Create(void *initialData);
			void Update(void *newData, VkDeviceSize newDataSize);
			void UpdateAtOffset(size_t frameIndex, size_t offset, void *newData, VkDeviceSize dataSize);
			void Clear();

		private:
#pragma region Variables
			std::vector<VkBuffer> _buffers;
			std::vector<VkDeviceMemory> _memories;
			std::vector<void *> _mappedMemories;

			VkDeviceSize _size;
			VkBufferUsageFlags _usage;
			// VkMemoryPropertyFlags _memoryProprety;
#pragma endregion

			/* private */
	};
} // namespace Vox::Engine::Rendering::Buffers

#endif // __BUFFER_HPP__-