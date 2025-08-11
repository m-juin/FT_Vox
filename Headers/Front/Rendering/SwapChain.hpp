#ifndef __SWAPCHAIN_HPP__
#define __SWAPCHAIN_HPP__

#include <vector>
#include <vulkan/vulkan.h>

namespace SplineDesigner
{
	namespace Front
	{
		class Window;

		namespace Rendering
		{
			class Device;

			struct SwapChainSupportDetails
			{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> presentModes;
			};
			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice &device, VkSurfaceKHR &surface);

			class SwapChain
			{

				private:
					VkSwapchainKHR _swapChain;

					std::vector<VkImage> _images;
					std::vector<VkImageView> _imageViews;
					std::vector<VkFramebuffer> _framebuffers;

					VkFormat _imageFormat;
					VkExtent2D _extent;

					VkQueue _graphicQueue;
					VkQueue _presentQueue;

					void CreateSwapChain(Device *device, Window *win);
                    void CreateImageViews(VkImageAspectFlags aspectFlags, Device *device);
					/* private */
					
					Device *_device;
					SwapChain();

                    void CleanSwapChain();

				public:
					SwapChain(Device *device, Window *win);
					~SwapChain();

					void CreateFrameBuffer(VkRenderPass &renderPass, VkImageView &depthView);

					VkQueue &GetPresentQueue() {return this->_presentQueue;};
					VkQueue &GetGraphicQueue() {return this->_graphicQueue;};
					VkSwapchainKHR &GetVulkanInstance() {return this->_swapChain;};
					VkFramebuffer &GetFrameBuffer(uint8_t index) {return this->_framebuffers[index];};
					VkFormat GetFormat() {return this->_imageFormat;};
					VkExtent2D GetExtent() {return this->_extent;};
			};
		} // namespace Rendering

	} // namespace Front

} // namespace SplineDesigner

#endif // __SWAPCHAIN_HPP__