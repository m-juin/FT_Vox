#include "Front/Utils/TexturesAtlas.hpp"

#include <algorithm>

#include "stb/stb_image.h"

#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

namespace Vox::Front::Utils
{
	TexturesAtlas::TexturesAtlas(const std::vector<TexturesDatas> &&datas, uint32_t mainChannelCount)
		: ATexturesAtlas(mainChannelCount)
	{

		size_t size = datas.size();

		if (size == 0)
			throw std::runtime_error("Invalid masked atlas textures list!");
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
				throw std::runtime_error("Atlas size exceeds maximum texture dimensions!");
			this->_atlasWidth = width;
			this->_atlasHeight = height;
		}

		for (auto texture : datas)
		{
			int imgWidth, imgHeight, imgChannels;
			unsigned char *imgData =
				stbi_load(texture.path.c_str(), &imgWidth, &imgHeight, &imgChannels, this->_textureChannels);

			if (imgData != nullptr)
			{
				this->_textureWidth = imgWidth;
				this->_textureHeight = imgHeight;
				break;
			}
		}
		if (this->_textureWidth == 0)
			throw std::runtime_error("No valid texture in atlas!");
		this->BuildAtlas(std::move(datas));
	}

	void TexturesAtlas::BuildAtlas(const std::vector<TexturesDatas> &&datas)
	{
		this->_textureInfos.reserve(datas.size());

		size_t atlasSize = (_textureWidth * _textureHeight * _textureChannels) * nextPowerOfTwo(datas.size());

		std::unique_ptr<unsigned char[]> atlasData(new unsigned char[atlasSize]);

		std::fill(atlasData.get(), atlasData.get() + atlasSize, 255);

		size_t width = _textureWidth * this->_atlasWidth;
		size_t height = _textureHeight * this->_atlasHeight;
		this->_mainAtlas = std::make_unique<VulkanImage>(width, height);

		for (size_t y = 0; y < this->_atlasHeight; y++)
		{
			for (size_t x = 0; x < this->_atlasWidth; x++)
			{
				size_t index = y * this->_atlasWidth + x;
				if (index >= datas.size())
					continue;
				auto data = datas[index];
				const std::string &key = data.key;
				if (std::find_if(this->_textureInfos.begin(), this->_textureInfos.end(),
								 [key](TextureInfo &info) { return key == info.key; }) != this->_textureInfos.end())
					continue;
				size_t atlasPosY = y * this->_textureHeight;
				size_t atlasPosX = x * this->_textureWidth;

				TextureInfo info{
					data.key,
					static_cast<float>(x * _textureWidth) / width,
					static_cast<float>(y * _textureHeight) / height,
					static_cast<float>(_textureWidth) / width,
					static_cast<float>(_textureHeight) / height,
				};
				this->_textureInfos.push_back(info);

				unsigned char *imgData = this->LoadSingleTexture(data.path, this->_textureChannels);
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

		this->BuildImages(std::move(atlasData), atlasSize);
	}

	void TexturesAtlas::BuildImages(std::unique_ptr<unsigned char[]> mainData, VkDeviceSize mainSize)
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Front::Rendering::Utils::Buffers::Utils::CreateBuffer(
			Front::Rendering::Device::GetInstance().GetLogicalDevice(),
			Front::Rendering::Device::GetInstance().GetPhysicalDevice(), mainSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
			stagingBufferMemory);

		void *data;
		vkMapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, 0, mainSize, 0,
					&data);
		memcpy(data, mainData.get(), static_cast<size_t>(mainSize));
		vkUnmapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory);

		auto size = this->_mainAtlas->GetSize();

		this->_mainAtlas->CreateImage(
			VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, static_cast<uint32_t>(floor(log2(std::max(size[0], size[1]))) + 1));
		this->_mainAtlas->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		this->_mainAtlas->CopyBufferToImage(stagingBuffer);
		this->_mainAtlas->GenerateMipMap();

		vkDestroyBuffer(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, nullptr);

		this->_mainAtlas->CreateView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);
		this->_mainAtlas->CreateSampler();
	}
} // namespace Vox::Front::Utils
