#ifndef __VULKANIMAGE_HPP__
#define __VULKANIMAGE_HPP__

#include <vulkan/vulkan.h>

#include "MathGraphicalLib/Vectors/Vector2.hpp"

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
			
			MGL::Vectors::Vector2<uint16_t> GetSize() {return MGL::Vectors::Vector2<uint16_t>(this->_width, this->_height);};

			VulkanImage(const VulkanImage &) = delete;
			VulkanImage &operator=(const VulkanImage &) = delete;
			VulkanImage(VulkanImage &&) noexcept = default;
			VulkanImage &operator=(VulkanImage &&) noexcept = default;

		protected:
			void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
			void CopyBufferToImage(VkBuffer buffer);

			void CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
							 VkMemoryPropertyFlags properties);
			void CreateView(VkFormat format, VkImageAspectFlags aspectFlags);
			virtual void CreateSampler();

			uint16_t _width;
			uint16_t _height;

			VkImage _image;
			VkDeviceMemory _memory;

			VkImageView _view;
			VkSampler _sampler;
			/* private */
	};
} // namespace Vox::Front::Rendering::Images

#endif // __VULKANIMAGE_HPP__