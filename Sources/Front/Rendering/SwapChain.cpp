#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/QueueFamilyIndices.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/VulkanManager.hpp"
#include "Front/Rendering/Images/DepthImage.hpp"
#include "Front/Window.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>

#include <GLFW/glfw3.h>

namespace Vox::Front::Rendering
{
	SwapChain::SwapChain()
	{
		this->CreateSwapChain();

		Vox::Front::Rendering::Device &device = Device::GetInstance();

		QueueFamilyIndices indices =
			QueueFamilyIndices::findQueueFamilies(device.GetPhysicalDevice(), Window::GetInstance().GetSurface());

		vkGetDeviceQueue(device.GetLogicalDevice(), indices.graphicsFamily.value(), 0, &this->_graphicQueue);
		vkGetDeviceQueue(device.GetLogicalDevice(), indices.presentFamily.value(), 0, &this->_presentQueue);
	}

	SwapChain::~SwapChain()
	{
		this->CleanSwapChain();
	}

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice &device, VkSurfaceKHR &surface)
	{

		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0)
		{
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
	{
		for (const auto &availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
				availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
	{
		for (const auto &availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_FIFO_KHR)
			{
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		else
		{
			int width, height;
			glfwGetFramebufferSize(Window::GetInstance().GetWindow(), &width, &height);

			VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

			actualExtent.width =
				std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height =
				std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void SwapChain::CreateImageViews(VkImageAspectFlags aspectFlags)
	{
		this->_imageViews.resize(this->_images.size());

		for (size_t i = 0; i < this->_images.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = this->_images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = this->_imageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = aspectFlags;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(Device::GetInstance().GetLogicalDevice(), &createInfo, nullptr,
								  &this->_imageViews[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to create image views!");
		}
	}

	void SwapChain::CreateFrameBuffer(VkRenderPass &renderPass, VkImageView &depthView)
	{
		this->_framebuffers.resize(this->_imageViews.size());

		for (size_t i = 0; i < this->_imageViews.size(); i++)
		{
			std::array<VkImageView, 2> attachments = {this->_imageViews[i], depthView};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = this->_extent.width;
			framebufferInfo.height = this->_extent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(Device::GetInstance().GetLogicalDevice(), &framebufferInfo, nullptr,
									&this->_framebuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to create framebuffer!");
		}
	}

	void SwapChain::RecreateSwapChain()
	{
		vkDeviceWaitIdle(Device::GetInstance().GetLogicalDevice());
		this->CleanSwapChain();

		Front::Window &win = Window::GetInstance();
		int width = 0, height = 0;
		glfwGetFramebufferSize(win.GetWindow(), &width, &height);
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(win.GetWindow(), &width, &height);
			glfwWaitEvents();
		}

		this->CreateSwapChain();
		VulkanManager::GetInstance().SetDepthImage(new Rendering::Images::DepthImage());
		// DrawManager::GetInstance().CreateDepthImage();
		this->CreateFrameBuffer(Pipelines::PipelinesManager::GetInstance().GetRenderPass(), VulkanManager::GetInstance().GetDepthImage()->GetView());
	}

	void SwapChain::CreateSwapChain()
	{
		Vox::Front::Window &win = Window::GetInstance();
		Vox::Front::Rendering::Device &device = Device::GetInstance();
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device.GetPhysicalDevice(), win.GetSurface());

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
		{
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = win.GetSurface();

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices =
			QueueFamilyIndices::findQueueFamilies(device.GetPhysicalDevice(), win.GetSurface());
		uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

		if (indices.graphicsFamily != indices.presentFamily)
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else
		{
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(device.GetLogicalDevice(), &createInfo, nullptr, &this->_swapChain) != VK_SUCCESS)
			throw std::runtime_error("Failed to create swap chain!");

		vkGetSwapchainImagesKHR(device.GetLogicalDevice(), this->_swapChain, &imageCount, nullptr);
		this->_images.resize(imageCount);
		vkGetSwapchainImagesKHR(device.GetLogicalDevice(), this->_swapChain, &imageCount, this->_images.data());

		this->_imageFormat = surfaceFormat.format;
		this->_extent = extent;

		this->CreateImageViews(VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void SwapChain::CleanSwapChain()
	{
		Vox::Front::Rendering::Device &device = Device::GetInstance();

		for (auto framebuffer : this->_framebuffers)
			vkDestroyFramebuffer(device.GetLogicalDevice(), framebuffer, nullptr);
		for (auto imageView : this->_imageViews)
			vkDestroyImageView(device.GetLogicalDevice(), imageView, nullptr);
		
		vkDestroySwapchainKHR(device.GetLogicalDevice(), this->_swapChain, nullptr);
	}
} // namespace Vox::Front::Rendering