#ifndef __TRACYUTILS_HPP__
#define __TRACYUTILS_HPP__

#ifdef TRACY_ENABLE
#include "Tracy/include/tracy/TracyVulkan.hpp"
#include "Tracy/include/common/TracyColor.hpp"
#include <vulkan/vulkan.h>

#include "LoggerLib/UtilityFunctions.hpp"

namespace Vox::TracyUtils
{
    inline TracyVkCtx& GetTracyContext() {
        static TracyVkCtx context = nullptr;
        return context;
    }

    inline void initVulkanTracy(VkDevice &device, VkPhysicalDevice &physicalDevice, VkQueue &queue,
                                VkCommandBuffer &vkCMDBuffer)
    {
        GetTracyContext() = TracyVkContext(physicalDevice, device, queue, vkCMDBuffer);
        LoggerLib::LogInfo("Tracy context initialized.");
    }

    inline void cleanupVulkanTracy()
    {
        auto& context = GetTracyContext();
        if (context)
        {
            TracyVkDestroy(context);
            context = nullptr;
            LoggerLib::LogInfo("Tracy context cleaned.");
        }
    }

} // namespace Vox::TracyUtils
#endif
#endif // __TRACYUTILS_HPP__