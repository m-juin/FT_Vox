#include "Front/Rendering/Utils/Buffers/DynamicBuffer.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include "Utils/TracyUtils.hpp"
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
		if (this->_buffers.size() == 0)
			return;
		vkQueueWaitIdle(SwapChain::GetInstance().GetGraphicQueue());
		CleanUp();
	}

	void DynamicBuffer::CleanUp()
	{
		VkDevice &device = Device::GetInstance().GetLogicalDevice();
		for (size_t i = 0; i < _buffers.size(); i++)
		{
			vkDestroyBuffer(device, _buffers[i], nullptr);
			vkFreeMemory(device, _memories[i], nullptr);
		}
		this->_buffers.clear();
	}

	void DynamicBuffer::Create(void *initialData)
	{
		#ifdef TRACY_ENABLE
		ZoneScopedNC("dBuffer creation", tracy::Color::LawnGreen);
		#endif
		Device &device = Device::GetInstance();

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = _size;
		bufferInfo.usage = _usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		for (auto &buffer : _buffers)
		{
			#ifdef TRACY_ENABLE
			ZoneScopedNC("object creation ", tracy::Color::LawnGreen);
			#endif
			if (vkCreateBuffer(device.GetLogicalDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
			{
				throw std::runtime_error("Échec de la création du tampon dynamique");
			}
		}

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
			#ifdef TRACY_ENABLE
			ZoneScopedNC("dBuffer memory set ", tracy::Color::GreenYellow);
			#endif
			{
				#ifdef TRACY_ENABLE
				ZoneScopedNC("dBuffer memory allocation ", tracy::Color::GreenYellow);
				#endif
				if (vkAllocateMemory(device.GetLogicalDevice(), &allocInfo, nullptr, &_memories[i]) != VK_SUCCESS)
				{
					throw std::runtime_error("Échec de l'allocation de mémoire");
				}
			}
			{
				#ifdef TRACY_ENABLE
				ZoneScopedNC("dBuffer memory binding ", tracy::Color::GreenYellow);
				#endif
				vkBindBufferMemory(device.GetLogicalDevice(), _buffers[i], _memories[i], 0);
			}
			{
				#ifdef TRACY_ENABLE
				ZoneScopedNC("dBuffer memory mapping ", tracy::Color::GreenYellow);
				#endif
				vkMapMemory(device.GetLogicalDevice(), _memories[i], 0, _size, 0, &_mappedMemories[i]);
			}
			{
				#ifdef TRACY_ENABLE
				ZoneScopedNC("dBuffer memory data copy ", tracy::Color::GreenYellow);
				#endif
				if (initialData)
					std::memcpy(_mappedMemories[i], initialData, this->_size);
				else
					std::memset(_mappedMemories[i], 0, this->_size);
			}
		}
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

	void DynamicBuffer::UpdateAtOffset(size_t frameIndex, size_t offset, void *newData, VkDeviceSize dataSize)
	{
		if (offset + dataSize > _size)
		{
			throw std::runtime_error("UpdateAtOffset dépasse la taille du buffer !");
		}

		std::memcpy(reinterpret_cast<char *>(_mappedMemories[frameIndex]) + offset, newData, dataSize);
	}

} // namespace Vox::Front::Rendering::Utils::Buffers