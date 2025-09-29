#ifndef __TEXTURESATLAS_HPP__
#define __TEXTURESATLAS_HPP__

#include <iostream>
#include <vector>

#include "Front/Rendering/Images/VulkanImage.hpp"

#include "MathGraphicalLib/Vectors/Vector2.hpp"

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
				const std::string key;
				float uOffset;
				float vOffset;
				float uSize;
				float vSize;
			};

			std::vector<TextureInfo> _textureInfos;
			size_t _textureWidth;
			size_t _textureHeight;
			size_t _textureChannels;


		public:
			struct uvData
			{
				MGL::Vectors::Vector2<float> uvMin;
				MGL::Vectors::Vector2<float> uvMax;
				MGL::Vectors::Vector2<uint16_t> atlasSize;
				int textureID;
			};

			const std::vector<TextureInfo> &GetTextureInfo() const {return this->_textureInfos;}
			// TexturesAtlas(std::vector<std::string> atlasTextures, size_t textureSize = 32, size_t textureChannels = 4);
			TexturesAtlas(std::vector<std::pair<std::string, std::string>> &&atlasTextures,
						  size_t textureChannels = 4);
			const TextureInfo &GetTextureInfo(size_t index) const;
			const TextureInfo &GetTextureInfo(const std::string &key) const;
			// const MGL::Vectors::Vector2<size_t> GetAtlasSize();
		private:
			static uint32_t nextPowerOfTwo(uint32_t value);
			void BuildAtlas(std::vector<std::pair<std::string, std::string>> &&textures);
	};
} // namespace Vox::Front::Utils

#endif // __TEXTURESATLAS_HPP__