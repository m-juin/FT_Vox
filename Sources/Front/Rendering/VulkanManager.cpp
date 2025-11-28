#include "Front/Rendering/VulkanManager.hpp"
#include "Front/Rendering/ValidationLayers.hpp"

#include "Front/Rendering/Images/DepthImage.hpp"

#include <iostream>
#include <stdexcept>

#include <GLFW/glfw3.h>

#include "LoggerLib/UtilityFunctions.hpp"

namespace Vox::Front::Rendering
{
	void VulkanManager::SetDepthImage(Images::DepthImage *image)
	{
		if (this->_depthImage != nullptr)
			delete this->_depthImage;
		this->_depthImage = image;
	}

	std::vector<const char *> VulkanManager::GetRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char **glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		// extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
												 VkDebugUtilsMessageTypeFlagsEXT messageType,
												 const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
												 void *pUserData)
	{
		(void)messageSeverity;
		(void)messageType;
		(void)pUserData;

		LoggerLib::LogDebug("Validation layer: ", pCallbackData->pMessage);

		return VK_FALSE;
	}

	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
									 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
								 VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
	}

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
										  const VkAllocationCallbacks *pAllocator,
										  VkDebugUtilsMessengerEXT *pDebugMessenger)
	{
		auto func =
			(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		else
			return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	VkDebugUtilsMessengerEXT SetupDebugMessenger(VkInstance &instance)
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);
		VkDebugUtilsMessengerEXT debugMessenger = nullptr;

		auto vkCreateDebugUtilsMessengerEXT =
			(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (vkCreateDebugUtilsMessengerEXT != nullptr)
		{
			auto result = vkCreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger);
			if (result != VK_SUCCESS)
				throw std::runtime_error("failed to set up debug messenger!");
		}

		return debugMessenger;
	}

	VulkanManager::VulkanManager()
	{
		if (CheckValidationLayerSupport() == false)
			throw std::runtime_error("Validation layers requested, but not available!");

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Spline Designer";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = nullptr;

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;

		if (vkCreateInstance(&createInfo, nullptr, &this->_instance) != VK_SUCCESS)
			throw std::runtime_error("failed to create instance!");

		this->_debugMessenger = SetupDebugMessenger(this->_instance);
	}

	VulkanManager::~VulkanManager()
	{
		auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
			this->_instance, "vkDestroyDebugUtilsMessengerEXT");
		if (vkDestroyDebugUtilsMessengerEXT != nullptr)
		{
			vkDestroyDebugUtilsMessengerEXT(this->_instance, this->_debugMessenger, nullptr);
		}
		vkDestroyInstance(this->_instance, nullptr);
	}
} // namespace Vox::Front::Rendering