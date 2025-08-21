#include "Front/Rendering/Images/FontImage.hpp"

#include <cmath>

namespace Vox::Front::Rendering::Images
{
	TextureBounds FontImage::GetCharBounds(char ch) {}

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

        // creation of vulkan image
	}

	FontImage::~FontImage() {}

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

			_atlasSize[0] += face->glyph->bitmap.width + 1; // espace entre glyphes
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