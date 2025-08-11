#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/QueueFamilyIndices.hpp"
#include "Front/Window.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <array>

#include <GLFW/glfw3.h>

namespace SplineDesigner
{
	namespace Front
	{
		namespace Rendering
		{
			SwapChain::SwapChain(Device *device, Window *win)
			{
				this->CreateSwapChain(device, win);

				QueueFamilyIndices indices = QueueFamilyIndices::findQueueFamilies(device->GetPhysicalDevice(), win->GetSurface());

				vkGetDeviceQueue(device->GetLogicalDevice(), indices.graphicsFamily.value(), 0, &this->_graphicQueue);
				vkGetDeviceQueue(device->GetLogicalDevice(), indices.presentFamily.value(), 0, &this->_presentQueue);
				_device = device;
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
					vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
															  details.presentModes.data());
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

			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, Window *win)
			{
				if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
				{
					return capabilities.currentExtent;
				}
				else
				{
					int width, height;
					glfwGetFramebufferSize(win->GetWindow(), &width, &height);

					VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

					actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width,
													capabilities.maxImageExtent.width);
					actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
													 capabilities.maxImageExtent.height);

					return actualExtent;
				}
			}

			void SwapChain::CreateImageViews(VkImageAspectFlags aspectFlags, Device *device)
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

					if (vkCreateImageView(device->GetLogicalDevice(), &createInfo, nullptr, &this->_imageViews[i]) !=
						VK_SUCCESS)
						throw std::runtime_error("Failed to create image views!");
				}
			}

			void SwapChain::CreateFrameBuffer(VkRenderPass &renderPass, VkImageView &depthView)
			{
				this->_framebuffers.resize(this->_imageViews.size());

				for (size_t i = 0; i < this->_imageViews.size(); i++)
				{
					std::array<VkImageView, 2> attachments = {this->_imageViews[i],
															  depthView};

					VkFramebufferCreateInfo framebufferInfo{};
					framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
					framebufferInfo.renderPass = renderPass;
					framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
					framebufferInfo.pAttachments = attachments.data();
					framebufferInfo.width = this->_extent.width;
					framebufferInfo.height = this->_extent.height;
					framebufferInfo.layers = 1;

					if (vkCreateFramebuffer(this->_device->GetLogicalDevice(), &framebufferInfo, nullptr,
											&this->_framebuffers[i]) != VK_SUCCESS)
						throw std::runtime_error("Failed to create framebuffer!");
				}
			}

			void SwapChain::CreateSwapChain(Device *device, Window *win)
			{
				SwapChainSupportDetails swapChainSupport =
					QuerySwapChainSupport(device->GetPhysicalDevice(), win->GetSurface());

				VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
				VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
				VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, win);

				uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
				if (swapChainSupport.capabilities.maxImageCount > 0 &&
					imageCount > swapChainSupport.capabilities.maxImageCount)
				{
					imageCount = swapChainSupport.capabilities.maxImageCount;
				}

				VkSwapchainCreateInfoKHR createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
				createInfo.surface = win->GetSurface();

				createInfo.minImageCount = imageCount;
				createInfo.imageFormat = surfaceFormat.format;
				createInfo.imageColorSpace = surfaceFormat.colorSpace;
				createInfo.imageExtent = extent;
				createInfo.imageArrayLayers = 1;
				createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

				QueueFamilyIndices indices =
					QueueFamilyIndices::findQueueFamilies(device->GetPhysicalDevice(), win->GetSurface());
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
				createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
				createInfo.presentMode = presentMode;
				createInfo.clipped = VK_TRUE;

				createInfo.oldSwapchain = VK_NULL_HANDLE;

				if (vkCreateSwapchainKHR(device->GetLogicalDevice(), &createInfo, nullptr, &this->_swapChain) !=
					VK_SUCCESS)
					throw std::runtime_error("Failed to create swap chain!");

				vkGetSwapchainImagesKHR(device->GetLogicalDevice(), this->_swapChain, &imageCount, nullptr);
				this->_images.resize(imageCount);
				vkGetSwapchainImagesKHR(device->GetLogicalDevice(), this->_swapChain, &imageCount,
										this->_images.data());

				this->_imageFormat = surfaceFormat.format;
				this->_extent = extent;

				this->CreateImageViews(VK_IMAGE_ASPECT_COLOR_BIT, device);
			}

			void SwapChain::CleanSwapChain()
			{
				for (auto framebuffer : this->_framebuffers)
					vkDestroyFramebuffer(_device->GetLogicalDevice(), framebuffer, nullptr);
				for (auto imageView : this->_imageViews)
					vkDestroyImageView(_device->GetLogicalDevice(), imageView, nullptr);
				vkDestroySwapchainKHR(_device->GetLogicalDevice(), this->_swapChain, nullptr);
			}
		} // namespace Rendering
	} // namespace Front

} // namespace SplineDesigner