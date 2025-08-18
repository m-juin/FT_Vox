#include "Front/Utils/TexturesAtlas.hpp"

#include <cmath>

namespace Vox::Front::Utils
{
	uint32_t TexturesAtlas::nextPowerOfTwo(uint32_t value)
	{
		return 1U << (32 - __builtin_clz(value - 1));
	}

	TexturesAtlas::TexturesAtlas(std::vector<std::string> atlasTextures, size_t textureSize)
		: _atlasWidth(0), _atlasHeight(0), _textureSize(textureSize)
	{
		size_t size = atlasTextures.size();
		if (size == 0)
		{
			throw std::runtime_error("Invalid atlas textures list!");
		}
		else if (size == 1)
		{
			std::cout << "[WARNING] Atlas build with only one texture." << std::endl;
			this->_atlasHeight = 1;
			this->_atlasWidth = 1;
		}
		else
		{
			uint32_t width = nextPowerOfTwo(static_cast<uint32_t>(std::sqrt(size)));
			if (width > MAX_TEXTURE_SIZE)
			{
				throw std::runtime_error("Atlas size exceeds maximum texture dimensions!");
			}
			this->_atlasWidth = width;
			this->_atlasHeight = width;
		}
		BuildAtlas(std::move(atlasTextures));
	}

	TexturesAtlas::TexturesAtlas(std::vector<std::string> atlasTextures, uint32_t expectedWidth, size_t textureSize)
		: _atlasWidth(0), _atlasHeight(0), _textureSize(textureSize)
	{
		size_t size = atlasTextures.size();
		if (size == 0)
		{
			throw std::runtime_error("Invalid atlas textures list!");
		}
		else if (size == 1)
		{
			std::cout << "[WARNING] Atlas build with only one texture." << std::endl;
			this->_atlasHeight = 1;
			this->_atlasWidth = 1;
		}
		else
		{
			if (expectedWidth > MAX_TEXTURE_SIZE)
			{
				throw std::runtime_error("Expected width exceeds maximum texture dimensions!");
			}

			this->_atlasWidth = expectedWidth;
			this->_atlasHeight = static_cast<uint32_t>((size + expectedWidth - 1) / expectedWidth);
		}
		BuildAtlas(std::move(atlasTextures));
	}

	const TexturesAtlas::TextureInfo &TexturesAtlas::GetTextureInfo(size_t index) const
	{
		if (index >= _textureInfos.size())
		{
			throw std::out_of_range("Texture index out of bounds!");
		}
		return _textureInfos[index];
	}

	void TexturesAtlas::BuildAtlas(std::vector<std::string> &&textures)
	{
		this->_textureInfos.reserve(textures.size());

		float cellWidth = 1.0f / _atlasWidth;
		float cellHeight = 1.0f / _atlasHeight;

		for (size_t i = 0; i < textures.size(); ++i)
		{
			TextureInfo info;
			info.uOffset = (i % _atlasWidth) * cellWidth;
			info.vOffset = (i / _atlasWidth) * cellHeight;
			info.uSize = cellWidth;
			info.vSize = cellHeight;
			this->_textureInfos.push_back(info);
		}

        

		for (size_t y = 0; y < this->_atlasHeight; y++)
		{
			for (size_t x = 0; x < this->_atlasWidth; x++)
			{
				size_t index = y * this->_atlasHeight + x;
                size_t atlasPosY = y * this->_textureSize;
                size_t atlasPosX = x * this->_textureSize;
			}
		}
	}
} // namespace Vox::Front::Utils