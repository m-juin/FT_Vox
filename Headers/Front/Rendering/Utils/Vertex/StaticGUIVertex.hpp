#ifndef __STATICGUIVERTEX_HPP__
#define __STATICGUIVERTEX_HPP__

#include <array>
#include <vulkan/vulkan.h>

#include "MathGraphicalLib/Vectors/Vector2.hpp"
#include "MathGraphicalLib/Vectors/Vector4.hpp"

namespace Vox::Front::Rendering::Utils::Vertex
{
	enum class E_ImageType {
		Font = 0,
		Texture = 1,
		Color = 2
	};

	struct StaticGUIVertex
	{
			MGL::Vectors::Vector2<float> position;
			MGL::Vectors::Vector2<float> texCoord;
			MGL::Vectors::Vector4<float> texColor;
			E_ImageType imageType;

			StaticGUIVertex(MGL::Vectors::Vector2<float> position, MGL::Vectors::Vector2<float> texCoord,
							MGL::Vectors::Vector4<float> texColor, E_ImageType imageType_ = E_ImageType::Texture)
				: position(position), texCoord(texCoord), texColor(texColor), imageType(imageType_) {};
			StaticGUIVertex()
				: position(MGL::Vectors::Vector2<float>(0.0f)),
				  texCoord(MGL::Vectors::Vector2<float>(0.0f)),
				  texColor(MGL::Vectors::Vector4<float>(1.0f)),
				  imageType(E_ImageType::Texture) {};

			bool operator==(const StaticGUIVertex &m1) const
			{
				return (this->position == m1.position && this->texCoord == m1.texCoord &&
						this->texColor == m1.texColor && imageType == m1.imageType);
			}

			static VkVertexInputBindingDescription GetBindingDescription()
			{
				VkVertexInputBindingDescription bindingDescription{};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(StaticGUIVertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions()
			{
				std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};
				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(StaticGUIVertex, position);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(StaticGUIVertex, texCoord);

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(StaticGUIVertex, texColor);

				attributeDescriptions[3].binding = 0;
				attributeDescriptions[3].location = 3;
				attributeDescriptions[3].format = VK_FORMAT_R8_UINT;
				attributeDescriptions[3].offset = offsetof(StaticGUIVertex, imageType);

				return attributeDescriptions;
			}
	};
} // namespace Vox::Front::Rendering::Utils::Vertex

#endif // __STATICGUIVERTEX_HPP__