#ifndef __STATICGUIVERTEX_HPP__
#define __STATICGUIVERTEX_HPP__

#include <array>
#include <vulkan/vulkan.h>

#include "MathGraphicalLib/Vectors/Vector2.hpp"
#include "MathGraphicalLib/Vectors/Vector4.hpp"

namespace Vox::Front::Rendering::Utils::Vertex
{
	struct StaticGUIVertex
	{
			MGL::Vectors::Vector2<float> position;
			MGL::Vectors::Vector2<float> texCoord;
			MGL::Vectors::Vector4<float> texColor;

			StaticGUIVertex(MGL::Vectors::Vector2<float> position, MGL::Vectors::Vector2<float> texCoord,
							MGL::Vectors::Vector4<float> texColor)
				: position(position), texCoord(texCoord), texColor(texColor) {};
			StaticGUIVertex()
				: position(MGL::Vectors::Vector2<float>(0.0f)),
				  texCoord(MGL::Vectors::Vector2<float>(0.0f)),
				  texColor(MGL::Vectors::Vector4<float>(1.0f)) {};

			bool operator==(const StaticGUIVertex &m1) const
			{
				return (this->position == m1.position && this->texCoord == m1.texCoord &&
						this->texColor == m1.texColor);
			}

			static VkVertexInputBindingDescription GetBindingDescription()
			{
				VkVertexInputBindingDescription bindingDescription{};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(StaticGUIVertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
			{
				std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
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

				return attributeDescriptions;
			}
	};
} // namespace Vox::Front::Rendering::Utils::Vertex

#endif // __STATICGUIVERTEX_HPP__