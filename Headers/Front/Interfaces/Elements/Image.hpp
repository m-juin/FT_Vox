#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include "Bases/AElement.hpp"

#include "Front/Rendering/Utils/Vertex/StaticGUIVertex.hpp"

#include "Front/Utils/TexturesAtlas.hpp"

#include "MathGraphicalLib/Vectors/Defines.hpp"

#include "./Utils/Defines.hpp"

using E_ImageType = Vox::Front::Rendering::Utils::Vertex::E_ImageType;

namespace Vox::Front::Interfaces::Elements
{
	class Image : public Bases::AElement
	{
		private:

			Vertex _vertex[4];
			dbuffer *B_Vertices = nullptr;
			dbuffer *B_Indices = nullptr;
			/* data */

            void CleanBuffers(size_t mode);
            void ResetVertex() override;

			std::string _atlas;
			std::string _atlasKey;

			Color _colorMod;
			Vector2Float _textureRepeat;

			Front::Utils::TexturesAtlas::TexturesAtlas::uvData _uvMappingData;
		public:
			Image(std::string atlas = "", std::string key = "", Vector2Float pos = {0, 0}, Vector2Float size = {100, 50}, Color colorMod = {1.0, 1.0, 1.0, 1.0}, Vector2Float textureRepeat = {1, 1});
			~Image();

			void Draw() override;
			void SetPos(const Vector2Float newPos) override;
			void SetSize(const Vector2Float newSize) override;

			void SetColor(const Color &newColor);
			void SetTexture(const std::string &newAtlas, const std::string &newKey);
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __IMAGE_HPP__