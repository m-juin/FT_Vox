#include "Front/Rendering/Utils/Buffers/DynamicBuffer.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/SwapChain.hpp"
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

	DynamicBuffer::~DynamicBuffer()
	{
		vkQueueWaitIdle(SwapChain::GetInstance().GetGraphicQueue());
		VkDevice &device = Device::GetInstance().GetLogicalDevice();
		for (size_t i = 0; i < _buffers.size(); i++)
		{
			vkDestroyBuffer(device, _buffers[i], nullptr);
			vkFreeMemory(device, _memories[i], nullptr);
		}
	}

	void DynamicBuffer::Create(void *initialData)
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
			memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			device.GetPhysicalDevice());

		for (size_t i = 0; i < _buffers.size(); i++)
		{
			if (vkAllocateMemory(device.GetLogicalDevice(), &allocInfo, nullptr, &_memories[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("Échec de l'allocation de mémoire");
			}
			vkBindBufferMemory(device.GetLogicalDevice(), _buffers[i], _memories[i], 0);
			vkMapMemory(device.GetLogicalDevice(), _memories[i], 0, _size, 0, &_mappedMemories[i]);
		}

		size_t nextFrame = 0;
		if (this->_memories.size() != 1)
			nextFrame = SyncObjects::GetInstance().GetNextFrame();

		std::memcpy(_mappedMemories[nextFrame], initialData, this->_size);
		// std::memcpy(_mappedMemories[SyncObjects::GetInstance().GetNextFrame()], initialData, this->_size);
	}

	void DynamicBuffer::Update(void *newData, VkDeviceSize newDataSize)
	{
		size_t nextFrame = 0;
		if (_memories.size() != 1)
			nextFrame = SyncObjects::GetInstance().GetNextFrame();

		if (newDataSize > _size)
		{
			VkDevice device = Device::GetInstance().GetLogicalDevice();
			vkDeviceWaitIdle(Device::GetInstance().GetLogicalDevice());
			for (size_t i = 0; i < _buffers.size(); i++)
			{
				vkDestroyBuffer(device, _buffers[i], nullptr);
				vkFreeMemory(device, _memories[i], nullptr);
			}

			_size = newDataSize;

			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = _size;
			bufferInfo.usage = _usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			for (auto &buffer : _buffers)
			{
				if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
				{
					throw std::runtime_error("Échec de la recréation du tampon dynamique");
				}
			}

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device, _buffers[0], &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex =
				Utils::FindMemoryType(memRequirements.memoryTypeBits,
									  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
									  Device::GetInstance().GetPhysicalDevice());

			for (size_t i = 0; i < _buffers.size(); i++)
			{
				vkAllocateMemory(device, &allocInfo, nullptr, &_memories[i]);
				vkBindBufferMemory(device, _buffers[i], _memories[i], 0);
				vkMapMemory(device, _memories[i], 0, _size, 0, &_mappedMemories[i]);
			}
		}

		std::memcpy(_mappedMemories[nextFrame], newData, newDataSize);
	}

} // namespace Vox::Front::Rendering::Utils::Buffers