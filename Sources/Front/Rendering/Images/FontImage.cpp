#include "Front/Rendering/Images/FontImage.hpp"

#include <cmath>

#include "Front/Rendering/Device.hpp"

#include "Front/Rendering/Utils/Buffers/Utils.hpp"

namespace Vox::Front::Rendering::Images
{
	TextureBounds FontImage::GetCharBounds(char ch)
	{
		Character chData = this->_fontDictionnary[ch];

		TextureBounds bounds;

		bounds.topLeft[1] = float(chData.y / float(this->_atlasSize[1]));
		bounds.bottomRight[1] = float((chData.y + chData.height) / float(this->_atlasSize[1]));

		bounds.topLeft[0] = float(chData.x / float(this->_atlasSize[0]));
		bounds.bottomRight[0] = float((chData.x + chData.width) / float(this->_atlasSize[0]));

		return bounds;
	}

	FontImage::FontImage(const std::string &fontPath) : VulkanImage(0, 0)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			throw std::runtime_error("Failed To Init freetype!");

		FT_Face face;

		if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
			throw std::runtime_error("Failed To parse font!");

		this->CreateAtlas(face);

		this->FillAtlas(face);

		this->_width = this->_atlasSize[0];
		this->_height = this->_atlasSize[1];

		size_t imgSize = this->_width * this->_height;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		Front::Rendering::Utils::Buffers::Utils::CreateBuffer(
			Front::Rendering::Device::GetInstance().GetLogicalDevice(),
			Front::Rendering::Device::GetInstance().GetPhysicalDevice(), imgSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
			stagingBufferMemory);

		void *data;
		vkMapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, 0, imgSize, 0,
					&data);

		memcpy(data, this->_atlas.data(), static_cast<size_t>(imgSize));
		vkUnmapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory);

		this->CreateImage(VK_FORMAT_R8_UNORM, VK_IMAGE_TILING_OPTIMAL,
						  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		this->CopyBufferToImage(stagingBuffer);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		vkDestroyBuffer(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, nullptr);

		this->CreateView(VK_FORMAT_R8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
		this->CreateSampler();
	}

	void FontImage::CreateAtlas(FT_Face &face)
	{
		_atlasSize[0] = 0;
		_atlasSize[1] = 0;
		for (char c = 32; c < 127; ++c)
		{
			FT_Set_Pixel_Sizes(face, 0, 64);

			_fontAscent = face->size->metrics.ascender >> 6;
			_fontDescent = face->size->metrics.descender >> 6;

			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			_atlasSize[0] += face->glyph->bitmap.width + 1;
			_atlasSize[1] = std::max(_atlasSize[1], (size_t)face->glyph->bitmap.rows);
		}

		this->_atlas = std::vector<uint8_t>(_atlasSize[0] * _atlasSize[1], 0);
	}

	void FontImage::FillAtlas(FT_Face &face)
	{
		uint32_t xOffset = 0;
		for (char c = 32; c < 127; ++c)
		{
			FT_Set_Pixel_Sizes(face, 0, 64);

			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				continue;

			FT_Bitmap &bmp = face->glyph->bitmap;

			for (unsigned int y = 0; y < bmp.rows; ++y)
			{
				for (unsigned int x = 0; x < bmp.width; ++x)
				{
					this->_atlas[(y * this->_atlasSize[0]) + xOffset + x] = bmp.buffer[y * bmp.pitch + x];
				}
			}

			Character info;
			info.x = xOffset;
			info.y = 0;
			info.width = bmp.width;
			info.height = bmp.rows;
			info.bearingX = face->glyph->bitmap_left;
			info.bearingY = face->glyph->bitmap_top;
			info.advance = face->glyph->advance.x >> 6;

			_fontDictionnary[c] = info;

			xOffset += bmp.width + 1;
		}
	}
} // namespace Vox::Front::Rendering::Images