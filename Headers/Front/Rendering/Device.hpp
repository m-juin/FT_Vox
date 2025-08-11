#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#include <vulkan/vulkan.h>

#include <vector>

namespace Vox::Front::Rendering
{
	const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	class Device
	{
		private:
			VkPhysicalDevice _physicalDevice;
			VkDevice _logicalDevice;

			Device();

		public:
			VkPhysicalDevice &GetPhysicalDevice()
			{
				return _physicalDevice;
			};
			VkDevice &GetLogicalDevice()
			{
				return _logicalDevice;
			};

			Device(VkInstance &instance, VkSurfaceKHR &surface);
			~Device();
	};
}

#endif // __DEVICE_HPP__