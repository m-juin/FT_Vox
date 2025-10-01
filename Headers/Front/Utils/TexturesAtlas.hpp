#ifndef __TEXTURESATLAS_HPP__
#define __TEXTURESATLAS_HPP__

#include "Game/Utils/TexturesData.hpp"

#include "./ATexturesAtlas.hpp"

#include "Front/Rendering/Images/VulkanImage.hpp"

#include <memory>
#include <vector>

namespace Vox::Front::Utils
{
	class TexturesAtlas : public ATexturesAtlas
	{
		private:
			void BuildAtlas(const std::vector<ATexturesAtlas> &&datas);
			void BuildImages(std::unique_ptr<unsigned char[]> mainData, std::unique_ptr<unsigned char[]> maskData,
							 VkDeviceSize mainSize, VkDeviceSize maskSize);

		public:
			TexturesAtlas(const std::vector<MaskedTexturesDatas> &&datas, uint32_t mainChannelCount = 4);
			TexturesAtlas() = delete;
			~TexturesAtlas() {};

			/* private */
	};
} // namespace Vox::Front::Utils

#endif // __TEXTURESATLAS_HPP__