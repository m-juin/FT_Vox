#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include "Bases/AElement.hpp"

#include "Front/Rendering/Utils/Vertex/StaticGUIVertex.hpp"

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
		public:
			// Image(Vector2 pos = {0, 0}, Vector2 size = {100, 50});
			Image(std::string atlas = "", std::string key = "", Vector2 pos = {0, 0}, Vector2 size = {100, 50}, Color colorMod = {1.0, 1.0, 1.0, 1.0});
			~Image();

			void Draw() override;
			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;

			void SetColor(const Color &newColor);
			void SetTexture(const std::string &newAtlas, const std::string &newKey);
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __IMAGE_HPP__