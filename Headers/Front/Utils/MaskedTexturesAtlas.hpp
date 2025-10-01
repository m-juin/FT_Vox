#ifndef __MASKEDTEXTURESATLAS_HPP__
#define __MASKEDTEXTURESATLAS_HPP__

#include "Game/Utils/TexturesData.hpp"

#include "./ATexturesAtlas.hpp"

#include "Front/Rendering/Images/VulkanImage.hpp"

#include <memory>

namespace Vox::Front::Utils
{
	class MaskedTexturesAtlas : public ATexturesAtlas
	{
		private:
			struct MaskedTexturesDatas : TexturesDatas
			{
					const std::string maskPath;

					MaskedTexturesDatas(const std::string &key_, const std::string &path_,
										const std::string &maskPath_ = "")
						: TexturesDatas(key_, path_), maskPath(maskPath_) {};
			};

            void BuildAtlas(const std::vector<MaskedTexturesDatas> &&datas);
            void BuildImages(std::unique_ptr<unsigned char[]> mainData, std::unique_ptr<unsigned char[]> maskData, VkDeviceSize mainSize, VkDeviceSize maskSize);

		public:
			std::unique_ptr<VulkanImage> _maskAtlas;

			MaskedTexturesAtlas(const std::vector<MaskedTexturesDatas> &&datas, uint32_t mainChannelCount = 4);
			MaskedTexturesAtlas() = delete;
			~MaskedTexturesAtlas() {};

			/* private */
	};
} // namespace Vox::Front::Utils

#endif // __MASKEDTEXTURESATLAS_HPP__