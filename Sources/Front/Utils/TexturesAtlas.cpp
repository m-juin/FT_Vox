#include "Front/Utils/TexturesAtlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Ext/stb/stb_image.h"

#include <cmath>

namespace Vox::Front::Utils
{
	uint32_t TexturesAtlas::nextPowerOfTwo(uint32_t value)
	{
		return 1U << (32 - __builtin_clz(value - 1));
	}

	TexturesAtlas::TexturesAtlas(std::vector<std::string> atlasTextures, size_t textureSize, size_t textureChannels)
		: _atlasWidth(0), _atlasHeight(0), _textureSize(textureSize), _textureChannels(textureChannels)
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

	TexturesAtlas::TexturesAtlas(std::vector<std::string> atlasTextures, uint32_t expectedWidth, size_t textureSize,
								 size_t textureChannels)
		: _atlasWidth(0), _atlasHeight(0), _textureSize(textureSize), _textureChannels(textureChannels)
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

	void TexturesAtlas::BuildVkImage(unsigned char *imgData)
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		VkDeviceSize imgSize = this->_.x * this->_size.y * this->_imgChannels;
		Buffer::CreateBuffer(imgSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
							 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
							 stagingBufferMemory);

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = this->_atlasWidth * this->_textureSize;
		imageInfo.extent.height = this->_atlasHeight * this->_textureSize;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = this->_textureChannels == 4 ? VK_FORMAT_R8G8B8A8_UNORM : VK_FORMAT_R8_UNORM;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create image!");
		}
	}

	void TexturesAtlas::BuildAtlas(std::vector<std::string> &&textures)
	{
		this->_textureInfos.reserve(textures.size());
		float cellWidth = 1.0f / _atlasWidth;
		float cellHeight = 1.0f / _atlasHeight;

		// Allocation de la mémoire pour l'atlas
		unsigned char atlasData[(this->_textureSize * this->_textureSize * this->_textureChannels) * textures.size()];

		// Remplissage initial avec des pixels blancs
		std::fill(atlasData,
				  atlasData + (this->_textureSize * this->_textureSize * this->_textureChannels * textures.size()),
				  255);

		for (size_t y = 0; y < this->_atlasHeight; y++)
		{
			for (size_t x = 0; x < this->_atlasWidth; x++)
			{
				size_t index = y * this->_atlasHeight + x;
				if (index >= textures.size())
					break;

				size_t atlasPosY = y * this->_textureSize;
				size_t atlasPosX = x * this->_textureSize;

				int imgWidth, imgHeight, imgChannels;
				const unsigned char *imgData =
					stbi_load(textures[index].c_str(), &imgWidth, &imgHeight, &imgChannels, 0);

				TextureInfo info;
				info.uOffset = x * cellWidth;
				info.vOffset = y * cellHeight;
				info.uSize = cellWidth;
				info.vSize = cellHeight;
				this->_textureInfos.push_back(info);

				if (imgData == NULL)
				{
					std::cout << "[WARNING] Failed to load image \"" << textures[index]
							  << "\". Using white placeholder." << std::endl;
					continue;
				}

				if (imgWidth != this->_textureSize || imgHeight != this->_textureSize ||
					imgChannels != this->_textureChannels)
				{
					std::cout << "[WARNING] Invalid image format \"" << textures[index]
							  << "\". Neither img size or channel number is invalid." << std::endl;
					stbi_image_free((void *)imgData);
					continue;
				}

				// Copie des pixels de l'image dans l'atlas
				for (size_t yTex = 0; yTex < this->_textureSize; yTex++)
				{
					for (size_t xTex = 0; xTex < this->_textureSize; xTex++)
					{
						size_t srcPos = (yTex * imgWidth + xTex) * this->_textureChannels;
						size_t dstPos = ((atlasPosY + yTex) * this->_textureSize * this->_textureChannels +
										 (atlasPosX + xTex) * this->_textureChannels);

						std::copy(imgData + srcPos, imgData + srcPos + this->_textureChannels, atlasData + dstPos);
					}
				}

				stbi_image_free((void *)imgData);
			}
		}
	}
} // namespace Vox::Front::Utils