#include "Front/Rendering/Images/DepthImage.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/SwapChain.hpp"

#include "Front/Rendering/Utils/Buffers/Utils.hpp"
#include "Front/Rendering/Utils/FindFormat.hpp"

#include <stdexcept>

namespace Vox::Front::Rendering::Images
{
	void DepthImage::CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
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

	void DepthImage::CreateView(VkFormat format, VkImageAspectFlags aspectFlags)
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

	void DepthImage::CreateSampler()
	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(Device::GetInstance().GetPhysicalDevice(), &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

		if (vkCreateSampler(Device::GetInstance().GetLogicalDevice(), &samplerInfo, nullptr, &this->_sampler) != VK_SUCCESS)
			throw std::runtime_error("Failed to create texture sampler!");
	}

	DepthImage::DepthImage()
	{

		this->_width = SwapChain::GetInstance().GetExtent().width;
		this->_height = SwapChain::GetInstance().GetExtent().height;
		VkFormat format = Utils::FindDepthFormat(Device::GetInstance().GetPhysicalDevice());
		this->CreateImage(format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		this->CreateView(format, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	DepthImage::~DepthImage()
	{
		std::cout << "Depth image deletion\n" << std::endl;
		VkDevice device = Device::GetInstance().GetLogicalDevice();
		vkDestroyImageView(device, this->_view, nullptr);
		vkDestroyImage(device, this->_image, nullptr);
	}

} // namespace Vox::Front::Rendering::Images