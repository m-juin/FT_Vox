#ifndef __DEVICE_HPP__
#define __DEVICE_HPP__

#include <vulkan/vulkan.h>

#include <vector>

#include "Utils/Singleton.hpp"
namespace Vox::Front::Rendering
{
	const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
	class Device : public Vox::Utils::Singleton<Device>
	{
		friend class Vox::Utils::Singleton<Device>;
		private:
			VkPhysicalDevice _physicalDevice;
			VkDevice _logicalDevice;

			Device();

			Device(VkInstance &instance, VkSurfaceKHR &surface);
		public:

			~Device();
			VkPhysicalDevice &GetPhysicalDevice()
			{
				return _physicalDevice;
			};
			VkDevice &GetLogicalDevice()
			{
				return _logicalDevice;
			};
	};
}

#endif // __DEVICE_HPP__