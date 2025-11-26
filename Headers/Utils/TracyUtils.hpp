#ifndef __TRACYUTILS_HPP__
#define __TRACYUTILS_HPP__

#define TRACY_ENABLE
#include "Tracy/include/tracy/tracy/TracyVulkan.hpp"

#include <vulkan/vulkan.h>

namespace Vox::TracyUtils
{
	static TracyVkCtx g_tracyVkContext = nullptr;

	static void initVulkanTracy(VkDevice &device, VkPhysicalDevice &physicalDevice, VkQueue &queue,
								VkCommandBuffer &vkCMDBuffer)
	{
		g_tracyVkContext = TracyVkContext(physicalDevice, device, queue, vkCMDBuffer);
	}

	static void cleanupVulkanTracy()
	{
		if (g_tracyVkContext)
		{
			TracyVkDestroy(g_tracyVkContext);
		}
	}


} // namespace Vox::TracyUtils
#endif // __TRACYUTILS_HPP__