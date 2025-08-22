#ifndef __FONTIMAGE_HPP__
#define __FONTIMAGE_HPP__

#include "./VulkanImage.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "MathGraphicalLib/Vectors/Vector2.hpp"

namespace Vox::Front::Rendering::Images
{
	template <typename T> using Vector2 = MGL::Vectors::Vector2<T>;
	struct Character
	{
			int x;
			int y;
			int width;
			int height;
			int bearingX;
			int bearingY;
			int advance;
	};

	struct TextureBounds
	{
			Vector2<float> topLeft;
			Vector2<float> bottomRight;
	};

	class FontImage : public VulkanImage
	{
		public:
			int GetAscent()
			{
				return this->_fontAscent;
			};
			int GetDescent()
			{
				return this->_fontDescent;
			};

			Character operator[](char ch)
			{
				return this->_fontDictionnary[ch];
			};
			TextureBounds GetCharBounds(char ch);

			FontImage() = delete;
			FontImage(const std::string &fontPath);
			~FontImage() {};

		private:
			void CreateAtlas(FT_Face &face);
			void FillAtlas(FT_Face &face);

			Vector2<size_t> _atlasSize;
			std::vector<uint8_t> _atlas;
			std::unordered_map<char, Character> _fontDictionnary;

			int _fontAscent = 0;
			int _fontDescent = 0;
			/* private */
	};
} // namespace Vox::Front::Rendering::Images

#endif // __FONTIMAGE_HPP__