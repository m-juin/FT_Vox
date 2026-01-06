#ifndef __DYNAMICIMAGE_HPP__
#define __DYNAMICIMAGE_HPP__

#include "./Bases/AElement.hpp"

#include "Front/Utils/TexturesAtlas.hpp"

#include "Front/Rendering/Images/VulkanImage.hpp"

#include "MathGraphicalLib/Vectors/Defines.hpp"

namespace Vox::Front::Interfaces::Elements
{
	using namespace MGL::Vectors::Types;
	class DynamicImage : public virtual Bases::AElement, public virtual Rendering::Images::VulkanImage
	{
		public:
			struct Constructor
			{
					Vector2Float pos;
					Vector2Float size;
                    Color colorMod;
					std::vector<uint8_t> defaultData;

					Constructor() : pos({0, 0}), size({50, 50}), colorMod({1.0, 1.0, 1.0, 1.0}) {};
			};

			DynamicImage() = delete;
			DynamicImage(const Constructor &st);
			void SetPos(const Vector2Float newPos) override;
			void SetSize(const Vector2Float newSize) override;

			void Draw() override;

			void SetData(std::vector<uint8_t> newData);

            void ResetVertex() override;

			~DynamicImage();

		private:

            void CreateVulkanImage();
            void UpdateVulkanImage();

			Vertex _vertex[4];
			dbuffer *B_Vertices = nullptr;
			dbuffer *B_Indices = nullptr;

			std::vector<uint8_t> _imgData;
            Front::Utils::TexturesAtlas::TexturesAtlas::uvData _uvMappingData;
            Color _colorMod;
			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __DYNAMICIMAGE_HPP__