#ifndef __DEPTHIMAGE_HPP__
#define __DEPTHIMAGE_HPP__

#include <vulkan/vulkan.h>

namespace Vox
{
	namespace Front
	{
		namespace Rendering
		{
			class Device;
			class SwapChain;
			namespace Images
			{
				class DepthImage
				{
					private:
						void CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
										 VkMemoryPropertyFlags properties);
						void CreateView(VkFormat format, VkImageAspectFlags aspectFlags);
						void CreateSampler();

						uint16_t _width;
						uint16_t _height;

						VkImage _image;
						VkDeviceMemory _memory;

						VkImageView _view;
						VkSampler _sampler;

						Device *_device;
						/* private */

						DepthImage();

					public:
						VkImageView &GetView() {return this->_view;};

						DepthImage(Device *device, SwapChain *swap);
						~DepthImage();
				};
			} // namespace Images
		} // namespace Rendering
	} // namespace Front
} // namespace Vox

#endif // __DEPTHIMAGE_HPP__