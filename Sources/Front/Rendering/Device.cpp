#include "Front/Rendering/Device.hpp"

#include <set>
#include <stdexcept>

#include "Front/Rendering/QueueFamilyIndices.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/ValidationLayers.hpp"

namespace Vox::Front::Rendering
{
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto &extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);

		return requiredExtensions.empty();
	}

	bool IsDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR &surface)
	{
		QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device, surface);

		bool extensionsSupported = CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device, surface);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	VkPhysicalDevice PickPhysicalDevice(VkInstance instance, VkSurfaceKHR &surface)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0)
			throw std::runtime_error("Failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		VkPhysicalDevice Phdevice;

		for (const auto &device : devices)
		{
			if (IsDeviceSuitable(device, surface))
			{
				Phdevice = device;
				break;
			}
		}

		if (Phdevice == VK_NULL_HANDLE)
			throw std::runtime_error("Failed to find a suitable GPU!");
		return Phdevice;
	}

	VkDevice CreateLogicalDevice(VkPhysicalDevice &device, VkSurfaceKHR &surface)
	{
		QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies)
		{
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		deviceFeatures.fillModeNonSolid = VK_TRUE;

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		VkDevice logicalDevice;

		if (vkCreateDevice(device, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
			throw std::runtime_error("Failed to create logical device!");

		return logicalDevice;
	}

	Device::Device(VkInstance &instance, VkSurfaceKHR &surface)
	{
		this->_physicalDevice = PickPhysicalDevice(instance, surface);
		this->_logicalDevice = CreateLogicalDevice(this->_physicalDevice, surface);
	}

	Device::~Device() {}
} // namespace Vox::Front::Rendering