#include "Front/Utils/TexturesAtlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <cmath>

#include "Front/Rendering/Device.hpp"

#include "Front/Rendering/Utils/Buffers/Utils.hpp"

namespace Vox::Front::Utils
{
	uint32_t TexturesAtlas::nextPowerOfTwo(uint32_t value)
	{
		return 1U << (32 - __builtin_clz(value - 1));
	}

	TexturesAtlas::TexturesAtlas(std::vector<std::string> atlasTextures, size_t textureSize, size_t textureChannels)
		: Rendering::Images::VulkanImage(0, 0), _atlasWidth(0), _atlasHeight(0), _textureSize(textureSize),
		  _textureChannels(textureChannels)
	{
		size_t size = atlasTextures.size();
		if (size == 0)
		{
			throw std::runtime_error("Invalid atlas textures list!");
		}
		else if (size == 1)
		{
			std::cout << "[WARNING] Atlas built with only one texture." << std::endl;
			this->_atlasWidth = 1;
			this->_atlasHeight = 1;
		}
		else
		{
			uint32_t width = static_cast<uint32_t>(std::ceil(std::sqrt(size)));
			uint32_t height = static_cast<uint32_t>(std::ceil(static_cast<float>(size) / width));

			if (width > MAX_TEXTURE_SIZE || height > MAX_TEXTURE_SIZE)
			{
				throw std::runtime_error("Atlas size exceeds maximum texture dimensions!");
			}

			this->_atlasWidth = width;
			this->_atlasHeight = height;
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

		size_t atlasSize =
			(this->_textureSize * this->_textureSize * this->_textureChannels) * nextPowerOfTwo(textures.size());
		std::cout << nextPowerOfTwo(textures.size()) << std::endl;
		std::cout << atlasSize << std::endl;
		std::unique_ptr<unsigned char[]> atlasData(new unsigned char[atlasSize]);

		std::fill(atlasData.get(), atlasData.get() + atlasSize, 255);

		this->_width = _textureSize * this->_atlasWidth;
		this->_height = _textureSize * this->_atlasHeight;

		for (size_t y = 0; y < this->_atlasHeight; y++)
		{
			for (size_t x = 0; x < this->_atlasWidth; x++)
			{
				size_t index = y * this->_atlasWidth + x;
				if (index >= textures.size())
					continue;

				size_t atlasPosY = y * this->_textureSize;
				size_t atlasPosX = x * this->_textureSize;

				TextureInfo info;
				info.uOffset = static_cast<float>(x) / _atlasWidth;
				info.vOffset = static_cast<float>(y) / _atlasHeight;
				info.uSize = 1.0f / _atlasWidth;
				info.vSize = 1.0f / _atlasHeight;
				this->_textureInfos.push_back(info);

				int imgWidth, imgHeight, imgChannels;
				unsigned char *imgData =
					stbi_load(textures[index].c_str(), &imgWidth, &imgHeight, &imgChannels, this->_textureChannels);

				if (!imgData)
				{
					std::cout << "[WARNING] Failed to load image \"" << textures[index]
							  << "\". Using white placeholder." << std::endl;
					continue;
				}

				if ((size_t)imgWidth != this->_textureSize || (size_t)imgHeight != this->_textureSize)
				{
					std::cout << "[WARNING] Invalid image format \"" << textures[index]
							  << "\". Wrong size or channels, skipping." << std::endl;
					stbi_image_free(imgData);
					continue;
				}

				for (size_t yTex = 0; yTex < this->_textureSize; yTex++)
				{
					size_t srcPos = (yTex * this->_textureSize * this->_textureChannels);
					size_t dstPos = ((atlasPosY + yTex) * (_atlasWidth * this->_textureSize * this->_textureChannels)) +
									(atlasPosX * _textureChannels);

					std::copy(imgData + srcPos, imgData + srcPos + (this->_textureChannels * _textureSize),
							  atlasData.get() + dstPos);
				}
				stbi_image_free(imgData);
			}
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		Front::Rendering::Utils::Buffers::Utils::CreateBuffer(
			Front::Rendering::Device::GetInstance().GetLogicalDevice(),
			Front::Rendering::Device::GetInstance().GetPhysicalDevice(), atlasSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
			stagingBufferMemory);

		void *data;
		vkMapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, 0, atlasSize, 0,
					&data);

		memcpy(data, atlasData.get(), static_cast<size_t>(atlasSize));
		vkUnmapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory);

		this->CreateImage(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
						  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		this->CopyBufferToImage(stagingBuffer);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, nullptr);

		this->CreateView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		this->CreateSampler();
	}
} // namespace Vox::Front::Utils