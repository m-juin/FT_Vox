#ifndef __TEXTURESATLAS_HPP__
#define __TEXTURESATLAS_HPP__

#include "Game/Datas/TexturesData.hpp"

#include "./ATexturesAtlas.hpp"

#include <memory>
#include <vector>

namespace Vox::Front::Utils
{
	class TexturesAtlas : public ATexturesAtlas
	{
		private:
			void BuildAtlas(const std::vector<TexturesDatas> &&datas);
			void BuildImages(std::unique_ptr<unsigned char[]> mainData, VkDeviceSize mainSize);

		public:
			TexturesAtlas(const std::vector<TexturesDatas> &&datas, uint32_t mainChannelCount = 4);
			TexturesAtlas() = delete;
			~TexturesAtlas() {};

			/* private */
	};
} // namespace Vox::Front::Utils

#endif // __TEXTURESATLAS_HPP__