#ifndef __TRACYUTILS_HPP__
#define __TRACYUTILS_HPP__

#ifdef TRACY_ENABLE

#include "Tracy/include/tracy/TracyVulkan.hpp"
#include <vulkan/vulkan.h>

namespace Vox::TracyUtils
{
    // Fonction pour obtenir la référence unique
    inline TracyVkCtx& GetTracyContext() {
        static TracyVkCtx context = nullptr;
        return context;
    }

    inline void initVulkanTracy(VkDevice &device, VkPhysicalDevice &physicalDevice, VkQueue &queue,
                                VkCommandBuffer &vkCMDBuffer)
    {
        GetTracyContext() = TracyVkContext(physicalDevice, device, queue, vkCMDBuffer);
    }

    inline void cleanupVulkanTracy()
    {
        auto& context = GetTracyContext();
        if (context)
        {
            TracyVkDestroy(context);
            context = nullptr;
        }
    }

} // namespace Vox::TracyUtils
#endif
#endif // __TRACYUTILS_HPP__