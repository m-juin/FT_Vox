#ifndef __BUFFERUTILS_HPP__
#define __BUFFERUTILS_HPP__

#include <stdexcept>
#include <vulkan/vulkan.h>

namespace Vox::Front::Rendering::Utils::Buffers::Utils
{
	inline uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice &device)
	{
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
			if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;

		throw std::runtime_error("Failed to find suitable memory type!");
	}

	inline void CreateBuffer(VkDevice logicalDevice, VkPhysicalDevice phDevice, VkDeviceSize size,
							 VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
							 VkDeviceMemory &bufferMemory)
	{
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
			throw std::runtime_error("Failed to create buffer!");

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, properties, phDevice);

		if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate buffer memory!");

		vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
	}

	inline VkCommandBuffer BeginSingleTimeCommands(VkDevice logicalDevice, VkCommandPool &pool)
	{
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = pool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	inline void EndSingleTimeCommands(VkCommandBuffer &commandBuffer, VkDevice &device, VkCommandPool &pool,
									  VkQueue &grQueue)
	{
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(grQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(grQueue);

		vkFreeCommandBuffers(device, pool, 1, &commandBuffer);
	}

	inline void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDevice device,
						   VkCommandPool &pool, VkQueue &grQueue)
	{
		VkCommandBuffer commandBuffer = BeginSingleTimeCommands(device, pool);

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		EndSingleTimeCommands(commandBuffer, device, pool, grQueue);
	}

} // namespace Vox::Front::Rendering::Utils::Buffer

#endif // __BUFFERUTILS_HPP__