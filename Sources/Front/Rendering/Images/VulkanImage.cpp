#include "Front/Rendering/Images/VulkanImage.hpp"

#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/SyncObjects.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

namespace Vox::Front::Rendering::Images
{
	VulkanImage::VulkanImage(size_t width, size_t height)
	{
		this->_sampler = VK_NULL_HANDLE;
		this->_memory = VK_NULL_HANDLE;
		this->_image = VK_NULL_HANDLE;
		this->_view = VK_NULL_HANDLE;
		this->_width = width;
		this->_height = height;
	}

	VulkanImage::~VulkanImage()
	{
		VkDevice device = Device::GetInstance().GetLogicalDevice();
		vkFreeMemory(device, this->_memory, nullptr);
		if (this->_sampler)
			vkDestroySampler(device, this->_sampler, nullptr);
		vkDestroyImageView(device, this->_view, nullptr);
		vkDestroyImage(device, this->_image, nullptr);
	}

	void VulkanImage::CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
								  VkMemoryPropertyFlags properties)
	{
		Front::Rendering::Device &device = Device::GetInstance();

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = this->_width;
		imageInfo.extent.height = this->_height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(device.GetLogicalDevice(), &imageInfo, nullptr, &this->_image) != VK_SUCCESS)
			throw std::runtime_error("failed to create image!");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device.GetLogicalDevice(), this->_image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Utils::Buffers::Utils::FindMemoryType(memRequirements.memoryTypeBits, properties,
																		  device.GetPhysicalDevice());

		if (vkAllocateMemory(device.GetLogicalDevice(), &allocInfo, nullptr, &this->_memory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(device.GetLogicalDevice(), this->_image, this->_memory, 0);
	}

	void VulkanImage::CreateView(VkFormat format, VkImageAspectFlags aspectFlags)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = this->_image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = aspectFlags;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(Device::GetInstance().GetLogicalDevice(), &viewInfo, nullptr, &this->_view) != VK_SUCCESS)
			throw std::runtime_error("Failed to create texture image view!!");
	}

	void VulkanImage::CreateSampler()
	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(Device::GetInstance().GetPhysicalDevice(), &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

		if (vkCreateSampler(Device::GetInstance().GetLogicalDevice(), &samplerInfo, nullptr, &this->_sampler) !=
			VK_SUCCESS)
			throw std::runtime_error("Failed to create texture sampler!");
	}


	void VulkanImage::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		VkCommandBuffer commandBuffer = Utils::Buffers::Utils::BeginSingleTimeCommands(Device::GetInstance().GetLogicalDevice(), CommandsPool::GetInstance().GetPool());

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;

		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.image = this->_image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
				 newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
				 newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else
			throw std::invalid_argument("Unsupported layout transition!");

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		Utils::Buffers::Utils::EndSingleTimeCommands(commandBuffer, Device::GetInstance().GetLogicalDevice(), CommandsPool::GetInstance().GetPool(), SwapChain::GetInstance().GetGraphicQueue());
	}

	void VulkanImage::CopyBufferToImage(VkBuffer buffer)
	{
		VkCommandBuffer commandBuffer = Utils::Buffers::Utils::BeginSingleTimeCommands(Device::GetInstance().GetLogicalDevice(), CommandsPool::GetInstance().GetPool());

		VkBufferImageCopy region{};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;

		region.imageOffset = {0, 0, 0};
		region.imageExtent = {this->_width, this->_height, 1};

		vkCmdCopyBufferToImage(commandBuffer, buffer, this->_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		Utils::Buffers::Utils::EndSingleTimeCommands(commandBuffer, Device::GetInstance().GetLogicalDevice(), CommandsPool::GetInstance().GetPool(), SwapChain::GetInstance().GetGraphicQueue());
	}

} // namespace Vox::Front::Rendering::Images