#ifndef __TEXTURESATLAS_HPP__
#define __TEXTURESATLAS_HPP__

#include <iostream>
#include <vector>

#include "Front/Rendering/Images/VulkanImage.hpp"

namespace Vox::Front::Utils
{
	class TexturesAtlas : public Rendering::Images::VulkanImage
	{
		private:
			static constexpr uint32_t MAX_TEXTURE_SIZE = 8192;
			uint32_t _atlasWidth;
			uint32_t _atlasHeight;

			struct TextureInfo
			{
				float uOffset;
				float vOffset;
				float uSize;
				float vSize;
			};

			std::vector<TextureInfo> _textureInfos;
			size_t _textureSize;
			size_t _textureChannels;


		public:
			// TexturesAtlas(std::vector<std::string> atlasTextures, size_t textureSize = 32, size_t textureChannels = 4);
			TexturesAtlas(std::vector<std::string> atlasTextures, size_t textureSize = 32,
						  size_t textureChannels = 4);
			const TextureInfo &GetTextureInfo(size_t index) const;

		private:
			static uint32_t nextPowerOfTwo(uint32_t value);
			void BuildAtlas(std::vector<std::string> &&textures);
	};
} // namespace Vox::Front::Utils

#endif // __TEXTURESATLAS_HPP__