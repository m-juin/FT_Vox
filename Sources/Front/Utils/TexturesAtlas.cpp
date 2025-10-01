#include "Front/Utils/TexturesAtlas.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#include <algorithm>
#include <cmath>

#include "Front/Rendering/Device.hpp"

#include "Front/Rendering/Utils/Buffers/Utils.hpp"

namespace Vox::Front::Utils
{
	uint32_t TexturesAtlas::nextPowerOfTwo(uint32_t value)
	{
		return 1U << (32 - __builtin_clz(value - 1));
	}

	TexturesAtlas::TexturesAtlas(std::vector<std::pair<std::string, std::string>> &&atlasTextures,
								 size_t textureChannels)
		: Rendering::Images::VulkanImage(0, 0), _atlasWidth(0), _atlasHeight(0), _textureWidth(0), _textureHeight(0),
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

		for (auto texture : atlasTextures)
		{
			int imgWidth, imgHeight, imgChannels;
			unsigned char *imgData =
				stbi_load(texture.second.c_str(), &imgWidth, &imgHeight, &imgChannels, this->_textureChannels);
			if (imgData != nullptr)
			{
				this->_textureWidth = imgWidth;
				this->_textureHeight = imgHeight;
				break;
			}
		}
		if (this->_textureWidth == 0)
			throw std::runtime_error("No valid texture in atlas!");

		BuildAtlas(std::move(atlasTextures));
	}

	const TextureInfo &TexturesAtlas::GetTextureInfo(size_t index) const
	{
		if (index >= _textureInfos.size())
		{
			throw std::out_of_range("Texture index out of bounds!");
		}
		return _textureInfos[index];
	}

	const TextureInfo &TexturesAtlas::GetTextureInfo(const std::string &key) const
	{
		auto it = std::find_if(this->_textureInfos.begin(), this->_textureInfos.end(),
							   [key](const TextureInfo &info) { return key == info.key; });
		if (it == this->_textureInfos.end())
		{
			throw std::out_of_range("Texture index out of bounds!");
		}
		return *it;
	}
	
	TexturesAtlas::~TexturesAtlas()
	{
		std::cout << "Texture Atlas\n";
	}

	void TexturesAtlas::BuildAtlas(std::vector<std::pair<std::string, std::string>> &&textures)
	{
		this->_textureInfos.reserve(textures.size());

		size_t atlasSize = (_textureWidth * _textureHeight * _textureChannels) * nextPowerOfTwo(textures.size());
		std::unique_ptr<unsigned char[]> atlasData(new unsigned char[atlasSize]);
		std::fill(atlasData.get(), atlasData.get() + atlasSize, 255);

		this->_width = _textureWidth * this->_atlasWidth;
		this->_height = _textureHeight * this->_atlasHeight;

		for (size_t y = 0; y < this->_atlasHeight; y++)
		{
			for (size_t x = 0; x < this->_atlasWidth; x++)
			{
				size_t index = y * this->_atlasWidth + x;
				if (index >= textures.size())
					continue;

				auto textKey = textures[index].first;
				if (std::find_if(this->_textureInfos.begin(), this->_textureInfos.end(), [textKey](TextureInfo &info)
								 { return textKey == info.key; }) != this->_textureInfos.end())
					continue;

				size_t atlasPosY = y * this->_textureHeight;
				size_t atlasPosX = x * this->_textureWidth;

				TextureInfo info{
					textures[index].first,
					static_cast<float>(x * _textureWidth) / _width,
					static_cast<float>(y * _textureHeight) / _height,
					static_cast<float>(_textureWidth) / _width,
					static_cast<float>(_textureHeight) / _height,
				};
				this->_textureInfos.push_back(info);

				int imgWidth, imgHeight, imgChannels;
				unsigned char *imgData = stbi_load(textures[index].second.c_str(), &imgWidth, &imgHeight, &imgChannels,
												   this->_textureChannels);

				if (!imgData)
				{
					std::cout << "[WARNING] Failed to load image \"" << textures[index].second
							  << "\". Using white placeholder." << std::endl;
					continue;
				}

				if ((size_t)imgWidth != this->_textureWidth || (size_t)imgHeight != this->_textureHeight)
				{
					std::cout << "[WARNING] Invalid image format \"" << textures[index].second
							  << "\". Wrong size or channels, skipping." << std::endl;
					stbi_image_free(imgData);
					continue;
				}
				for (size_t yTex = 0; yTex < this->_textureHeight; yTex++)
				{
					size_t srcPos = (yTex * this->_textureWidth * this->_textureChannels);
					size_t dstPos =
						((atlasPosY + yTex) * (_atlasWidth * this->_textureWidth * this->_textureChannels)) +
						(atlasPosX * _textureChannels);
					std::copy(imgData + srcPos, imgData + srcPos + (this->_textureChannels * _textureWidth),
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
						  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
						  static_cast<uint32_t>(floor(log2(std::max(this->_width, this->_height))) + 1));
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		this->CopyBufferToImage(stagingBuffer);
		this->GenerateMipMap();
		// this->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, nullptr);

		this->CreateView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		this->CreateSampler();
	}
} // namespace Vox::Front::Utils