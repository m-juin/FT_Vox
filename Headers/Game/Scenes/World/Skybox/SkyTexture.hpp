#ifndef __SKYTEXTURE_HPP__
#define __SKYTEXTURE_HPP__

#include "Front/Rendering/Images/VulkanImage.hpp"
#include "Front/Utils/ATexturesAtlas.hpp"

namespace Vox::Game::World::Skybox
{
	class SkyTexture : public Front::Rendering::Images::VulkanImage
	{
		public:
			SkyTexture();
			~SkyTexture();

		private:
			void CheckSBValidity(const std::string &path);
			void CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
							 VkMemoryPropertyFlags properties, uint32_t mipLevels = 1) override;
			void CopyBufferToImage(VkBuffer buffer) override;
			void CreateSampler() override;
			std::pair<std::array<unsigned char*, 6>, VkDeviceSize> LoadTextures(const std::string &path);
			std::pair<VkBuffer, VkDeviceMemory> CreateStagingBuffer(std::array<unsigned char*, 6>, VkDeviceSize);
			// void BuildAtlas(const std::string &path);
			// void BuildAtlas(const std::string &path);
			// void BuildImages(std::unique_ptr<unsigned char[]> mainData, VkDeviceSize mainSize);
			/* private */
	};

} // namespace Vox::Game::World::Skybox

#endif // __SKYTEXTURE_HPP__