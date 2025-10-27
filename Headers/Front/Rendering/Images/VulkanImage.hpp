#ifndef __VULKANIMAGE_HPP__
#define __VULKANIMAGE_HPP__

#include <vulkan/vulkan.h>

#include "MathGraphicalLib/Vectors/Vector2.hpp"

namespace Vox::Front::Utils
{
	class TexturesAtlas;
	class MaskedTexturesAtlas;
} // namespace Vox::Front::Utils

namespace Vox::Game::World::Skybox
{
	class SkyTexture;
} // namespace Game::Scene::World::Skybox


namespace Vox::Front::Rendering::Images
{
	class VulkanImage
	{
		friend class Vox::Front::Utils::TexturesAtlas;
		friend class Vox::Front::Utils::MaskedTexturesAtlas;
		friend class Vox::Game::World::Skybox::SkyTexture;
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
			void GenerateMipMap();

			void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
			virtual void CopyBufferToImage(VkBuffer buffer);

			virtual void CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
							 VkMemoryPropertyFlags properties, uint32_t mipLevels = 1);
			void CreateView(VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);
			virtual void CreateSampler();

			uint16_t _width;
			uint16_t _height;
			uint8_t _layerCount;

			VkImage _image;
			VkDeviceMemory _memory;

			VkImageView _view;
			VkSampler _sampler;

			uint32_t _mipLevels;
			/* private */
	};
} // namespace Vox::Front::Rendering::Images

#endif // __VULKANIMAGE_HPP__