#ifndef __SWAPCHAIN_HPP__
#define __SWAPCHAIN_HPP__

#include <vector>
#include <vulkan/vulkan.h>

#include "Utils/Singleton.hpp"

namespace Vox::Front
{
	namespace Rendering
	{

		struct SwapChainSupportDetails
		{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
		};
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice &device, VkSurfaceKHR &surface);

		class SwapChain : public Vox::Utils::Singleton<SwapChain>
		{
			friend class Vox::Utils::Singleton<SwapChain>;
			private:
				VkSwapchainKHR _swapChain;

				std::vector<VkImage> _images;
				std::vector<VkImageView> _imageViews;
				std::vector<VkFramebuffer> _framebuffers;

				VkFormat _imageFormat;
				VkExtent2D _extent;

				VkQueue _graphicQueue;
				VkQueue _presentQueue;

				void CreateSwapChain();
				void CreateImageViews(VkImageAspectFlags aspectFlags);
				/* private */
				SwapChain();

				void CleanSwapChain();

			public:
				~SwapChain();
				void CreateFrameBuffer(VkRenderPass &renderPass, VkImageView &depthView);

				VkQueue &GetPresentQueue()
				{
					return this->_presentQueue;
				};
				VkQueue &GetGraphicQueue()
				{
					return this->_graphicQueue;
				};
				VkSwapchainKHR &GetVulkanInstance()
				{
					return this->_swapChain;
				};
				VkFramebuffer &GetFrameBuffer(uint8_t index)
				{
					return this->_framebuffers[index];
				};
				VkFormat GetFormat()
				{
					return this->_imageFormat;
				};
				VkExtent2D GetExtent()
				{
					return this->_extent;
				};

				void RecreateSwapChain();
		};
	} // namespace Rendering

} // namespace Vox::Front

#endif // __SWAPCHAIN_HPP__