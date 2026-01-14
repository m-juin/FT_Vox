#include "Engine/Rendering/Buffers/Buffer.hpp"

#include "Front/Rendering/Utils/Buffers/Utils.hpp"
#include "Front/Rendering/Device.hpp"

namespace Vox::Engine::Rendering::Buffers
{
	using namespace Vox::Front::Rendering;
	Buffer::~Buffer()
	{
		if (this->_buffers.size() == 0)
			return;
		// vkQueueWaitIdle(SwapChain::GetInstance().GetGraphicQueue());
		CleanUp();
	}

	void Buffer::CleanUp()
	{
		VkDevice &device = Device::GetInstance().GetLogicalDevice();
		for (size_t i = 0; i < _buffers.size(); i++)
		{
			vkDestroyBuffer(device, _buffers[i], nullptr);
			vkFreeMemory(device, _memories[i], nullptr);
		}
		this->_buffers.clear();
	}

	void Buffer::Create(void *initialData)
	{
		// namespace FrontUtils = Vox::Front::Rendering::Utils::Buffers::Utils;

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

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.GetLogicalDevice(), _buffers[0], &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Vox::Front::Rendering::Utils::Buffers::Utils::FindMemoryType(
			memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			device.GetPhysicalDevice());

		for (size_t i = 0; i < _buffers.size(); i++)
		{
			if (vkAllocateMemory(device.GetLogicalDevice(), &allocInfo, nullptr, &_memories[i]) != VK_SUCCESS)
				throw std::runtime_error("Échec de l'allocation de mémoire");
			vkBindBufferMemory(device.GetLogicalDevice(), _buffers[i], _memories[i], 0);
			vkMapMemory(device.GetLogicalDevice(), _memories[i], 0, _size, 0, &_mappedMemories[i]);
			if (initialData)
				std::memcpy(_mappedMemories[i], initialData, this->_size);
			else
				std::memset(_mappedMemories[i], 0, this->_size);
		}
	}

	void Buffer::Update(void *newData, VkDeviceSize newDataSize)
	{
		(void)newData;
		(void)newDataSize;
	}

	void Buffer::UpdateAtOffset(size_t frameIndex, size_t offset, void *newData, VkDeviceSize dataSize)
	{
		(void)frameIndex;
		(void)offset;
		(void)newData;
		(void)dataSize;
	}

	void Buffer::Clear()
	{
		for (size_t i = 0; i < _mappedMemories.size(); i++)
		{
			std::memset(_mappedMemories[i], 0, _size);
		}
	}
} // namespace Vox::Engine::Rendering::Buffers