#ifndef __VOXELVERTEX_HPP__
#define __VOXELVERTEX_HPP__

#include "MathGraphicalLib/Vectors/Vector3.hpp"

#include <vulkan/vulkan.h>
#include <array>

namespace Vox::Front::Rendering::Utils::Vertex
{
	using Vector3Float = MGL::Vectors::Vector3<float>;
	struct VoxelVertex
	{
			Vector3Float vertPos;
			Vector3Float vertColor;

			VoxelVertex(Vector3Float vertPos_, Vector3Float vertColor_) : vertPos(vertPos_), vertColor(vertColor_) {};
			// VoxelVertex() : vertPos(Vector3Float(0.0f)), vertColor(Vector3Float(1.0f)) {};

			bool operator==(const VoxelVertex &v1)
			{
				return (this->vertPos == v1.vertPos && this->vertColor == v1.vertColor);
			}

			static VkVertexInputBindingDescription GetBindingDescription()
			{
				VkVertexInputBindingDescription bindingDescription{};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(VoxelVertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
			{
				std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(VoxelVertex, vertPos);

				attributeDescriptions[1].binding = 0;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(VoxelVertex, vertColor);

				return attributeDescriptions;
			}
	};

} // namespace Vox::Front::Rendering::Utils::Vertex

#endif // __VOXELVERTEX_HPP__