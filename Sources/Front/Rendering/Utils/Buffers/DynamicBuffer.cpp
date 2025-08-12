#include "Front/Rendering/Utils/Buffers/DynamicBuffer.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include <cstring>

namespace Vox::Front::Rendering::Utils::Buffers
{
	DynamicBuffer::DynamicBuffer(uint32_t frameCount, VkDeviceSize size, VkBufferUsageFlags usage)
		: ABuffer(frameCount), _mappedMemories(frameCount)
	{
		_size = size;
		_usage = usage;
	}

	DynamicBuffer::~DynamicBuffer() {}

	void DynamicBuffer::Create()
	{
		Device &device = Device::GetInstance();

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = _usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		for (auto &buffer : _buffers)
		{
			if (vkCreateBuffer(device.GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
			{
				throw std::runtime_error("Échec de la création du tampon dynamique");
			}
		}

		// Allocation de la mémoire pour chaque tampon
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.GetLogicalDevice(), _buffers[0], &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Utils::FindMemoryType(
			memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device.GetPhysicalDevice());

		for (size_t i = 0; i < _buffers.size(); i++)
		{
			if (vkAllocateMemory(device.GetLogicalDevice(), &allocInfo, nullptr, &_memories[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Échec de l'allocation de mémoire");
			}
			vkBindBufferMemory(device.GetLogicalDevice(), _buffers[i], _memories[i], 0);
			vkMapMemory(device.GetLogicalDevice(), _memories[i], 0, _size, 0, &_mappedMemories[i]);
		}
	}

	void DynamicBuffer::Update(void *newData, VkDeviceSize newDataSize)
    {
        this->_size = newDataSize;
        std::memcpy(_mappedMemories[SyncObjects::GetInstance().GetNextFrame()], newData, newDataSize);
    }
} // namespace Vox::Front::Rendering::Utils::Buffers