#ifndef __TRACYUTILS_HPP__
#define __TRACYUTILS_HPP__

#ifdef TRACY_ENABLE

#include "Tracy/include/tracy/TracyVulkan.hpp"

#include <vulkan/vulkan.h>

namespace Vox::TracyUtils
{
	static TracyVkCtx g_tracyVkContext = nullptr;

	static inline void initVulkanTracy(VkDevice &device, VkPhysicalDevice &physicalDevice, VkQueue &queue,
								VkCommandBuffer &vkCMDBuffer)
	{
		g_tracyVkContext = TracyVkContext(physicalDevice, device, queue, vkCMDBuffer);
	}

	static inline void cleanupVulkanTracy()
	{
		if (g_tracyVkContext)
		{
			TracyVkDestroy(g_tracyVkContext);
		}
	}

} // namespace Vox::TracyUtils
#endif
#endif // __TRACYUTILS_HPP__