#ifndef __VULKANIMAGE_HPP__
#define __VULKANIMAGE_HPP__

#include <vulkan/vulkan.h>

namespace Vox::Front::Rendering::Images
{
	class VulkanImage
	{
		public:
			VulkanImage(size_t width, size_t height);
			virtual ~VulkanImage();

			VkImageView &GetView()
			{
				return this->_view;
			};

			VkSampler &GetSampler()
			{
				return this->_sampler;
			};

		protected:
			void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
			void CopyBufferToImage(VkBuffer buffer);

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
			/* private */
	};
} // namespace Vox::front::Rendering::Images

#endif // __VULKANIMAGE_HPP__