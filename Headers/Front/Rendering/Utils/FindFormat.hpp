#ifndef __FINDFORMAT_HPP__
#define __FINDFORMAT_HPP__

#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

namespace Vox::Front::Rendering::Utils
{

	inline VkFormat FindSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
										VkFormatFeatureFlags features, VkPhysicalDevice &device)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(device, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
				return format;
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
				return format;
		}

		throw std::runtime_error("Failed to find supported format!");
	}

	inline VkFormat FindDepthFormat(VkPhysicalDevice &device)
	{
		return FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
								   VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT, device);
	}
} // namespace Vox::Front::Rendering::Utils
#endif // __FINDFORMAT_HPP__