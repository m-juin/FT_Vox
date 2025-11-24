#include "Front/Utils/ATexturesAtlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <algorithm>

#include "LoggerLib/UtilityFunctions.hpp"

namespace Vox::Front::Utils
{
	ATexturesAtlas::ATexturesAtlas(const uint32_t texturesChannels)
	{
		_atlasWidth = 0;
		_atlasHeight = 0;

		_textureWidth = 0;
		_textureHeight = 0;
		_textureChannels = texturesChannels;
	}

	uint32_t ATexturesAtlas::nextPowerOfTwo(uint32_t value)
    {
        return 1U << (32 - __builtin_clz(value - 1));
    }

	unsigned char *ATexturesAtlas::LoadSingleTexture(const std::string &path, uint8_t expectedChannel)
	{
		unsigned char *data = nullptr;

		if (path != "")
		{
			int imgWidth, imgHeight, imgChannels;
			data = stbi_load(path.c_str(), &imgWidth, &imgHeight, &imgChannels, expectedChannel);
			if (data == nullptr)
			{
				LoggerLib::LogWarning("Failed to load image \"", path, "\". Using default color.");
			}
			else if ((size_t)imgWidth != this->_textureWidth || (size_t)imgHeight != this->_textureHeight)
			{
				LoggerLib::LogWarning("Invalid image format \"", path, "\". Wrong size or channels, skipping.");
				stbi_image_free(data);
				data = nullptr;
			}
		}

		return data;
	}

	const TextureInfo &ATexturesAtlas::GetTextureInfo(size_t index) const
	{
		if (index >= _textureInfos.size())
		{
			throw std::out_of_range("Texture index out of bounds!");
		}
		return _textureInfos[index];
	}

	const TextureInfo &ATexturesAtlas::GetTextureInfo(const std::string &key) const
	{
		auto it = std::find_if(this->_textureInfos.begin(), this->_textureInfos.end(),
							   [key](const TextureInfo &info) { return key == info.key; });
		if (it == this->_textureInfos.end())
		{
			throw std::out_of_range("Texture index out of bounds!");
		}
		return *it;
	}
} // namespace Vox::Front::Utils
