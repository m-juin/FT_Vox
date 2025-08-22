#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

#include <cstring>

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

	void StaticBuffer::Create(void *initialData)
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

		/* ========== Staging Buffer for data copy ========== */
		

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		// Création du buffer temporaire
		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.size = _size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device.GetLogicalDevice(), &stagingBufferInfo, nullptr, &stagingBuffer) != VK_SUCCESS)
			throw std::runtime_error("Échec de la création du buffer temporaire.");

		// Allocation de la mémoire pour le buffer temporaire
		VkMemoryRequirements stagingMemRequirements;
		vkGetBufferMemoryRequirements(device.GetLogicalDevice(), stagingBuffer, &stagingMemRequirements);

		VkMemoryAllocateInfo stagingAllocInfo = {};
		stagingAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		stagingAllocInfo.allocationSize = stagingMemRequirements.size;
		stagingAllocInfo.memoryTypeIndex = Utils::FindMemoryType(
			stagingMemRequirements.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, device.GetPhysicalDevice());

		if (vkAllocateMemory(device.GetLogicalDevice(), &stagingAllocInfo, nullptr, &stagingBufferMemory) != VK_SUCCESS)
			throw std::runtime_error("Échec de l'allocation de la mémoire temporaire.");

		vkBindBufferMemory(device.GetLogicalDevice(), stagingBuffer, stagingBufferMemory, 0);

		// Copie des données dans le buffer temporaire
		void *data;
		vkMapMemory(device.GetLogicalDevice(), stagingBufferMemory, 0, _size, 0, &data);
		std::memcpy(data, initialData, static_cast<size_t>(_size));
		vkUnmapMemory(device.GetLogicalDevice(), stagingBufferMemory);

		// Copie des données depuis le buffer temporaire vers les buffers finaux
		auto pool = Vox::Front::Rendering::CommandsPool::GetInstance().GetPool();
		VkCommandBuffer copyCmd = Buffers::Utils::BeginSingleTimeCommands(device.GetLogicalDevice(), pool);

		for (auto &buffer : _buffers)
		{
			VkBufferCopy copyRegion = {};
			copyRegion.srcOffset = 0;
			copyRegion.dstOffset = 0;
			copyRegion.size = _size;

			vkCmdCopyBuffer(copyCmd, stagingBuffer, buffer, 1, &copyRegion);
		}

		Buffers::Utils::EndSingleTimeCommands(copyCmd, device.GetLogicalDevice(), pool, SwapChain::GetInstance().GetGraphicQueue());

		// Nettoyage des ressources temporaires
		vkDestroyBuffer(device.GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(device.GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void StaticBuffer::Update(void *newData, VkDeviceSize newDataSize)
	{
		(void)newData;
		(void)newDataSize;
		throw std::runtime_error("This error should not happen! Probably an error in the program.");
	}
} // namespace Vox::Front::Rendering::Utils::Buffers