#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

#include "Front/Rendering/Device.hpp"

namespace Vox::Front::Rendering::Utils::Buffers
{
	StaticBuffer::StaticBuffer(uint32_t frameCount, VkDeviceSize size, VkBufferUsageFlags usage) : ABuffer(frameCount)
	{
		_size = size;
        _usage = usage;
    }

	StaticBuffer::~StaticBuffer()
	{
		VkDevice &device = Device::GetInstance().GetLogicalDevice();
		for (size_t i = 0; i < _buffers.size(); i++)
		{
			vkDestroyBuffer(device, _buffers[i], nullptr);
			vkFreeMemory(device, _memories[i], nullptr);
		}
	}

	void StaticBuffer::Create()
	{
		Device &device = Device::GetInstance();

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = _usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		for (auto &buffer : _buffers)
			if (vkCreateBuffer(device.GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
				throw std::runtime_error("Failed to create the buffer.");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.GetLogicalDevice(), _buffers[0], &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Utils::FindMemoryType(
			memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, device.GetPhysicalDevice());

		for (size_t i = 0; i < _buffers.size(); i++)
		{
			if (vkAllocateMemory(device.GetLogicalDevice(), &allocInfo, nullptr, &_memories[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to allocate buffer memory.");
			vkBindBufferMemory(device.GetLogicalDevice(), _buffers[i], _memories[i], 0);
		}
	}

	void StaticBuffer::Update(void *newData, VkDeviceSize newDataSize)
	{
		throw std::runtime_error("This error should not happen! Probably an error in the program.");
	}
} // namespace Vox::Front::Rendering::Utils::Buffers