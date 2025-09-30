#ifndef __VOXELVERTEX_HPP__
#define __VOXELVERTEX_HPP__

#include "MathGraphicalLib/Vectors/Vector3.hpp"
#include "MathGraphicalLib/Vectors/Vector2.hpp"

#include <vulkan/vulkan.h>
#include <array>

namespace Vox::Front::Rendering::Utils::Vertex
{
	using Vector3Float = MGL::Vectors::Vector3<float>;
	using Vector2Float = MGL::Vectors::Vector2<float>;
	struct VoxelVertex
	{
			Vector3Float vertPos;
			Vector3Float vertColor;
			Vector2Float vertCoord;

			VoxelVertex(Vector3Float vertPos_, Vector3Float vertColor_, Vector2Float vertCoord_) : vertPos(vertPos_), vertColor(vertColor_), vertCoord(vertCoord_) {};
			// VoxelVertex() : vertPos(Vector3Float(0.0f)), vertColor(Vector3Float(1.0f)) {};

			bool operator==(const VoxelVertex &v1)
			{
				return (this->vertPos == v1.vertPos && this->vertColor == v1.vertColor && this->vertCoord == v1.vertCoord);
			}

			static VkVertexInputBindingDescription GetBindingDescription()
			{
				VkVertexInputBindingDescription bindingDescription{};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(VoxelVertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
			{
				std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(VoxelVertex, vertPos);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(VoxelVertex, vertColor);

				attributeDescriptions[2].binding = 0;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(VoxelVertex, vertCoord);

				return attributeDescriptions;
			}
	};

} // namespace Vox::Front::Rendering::Utils::Vertex

#endif // __VOXELVERTEX_HPP__