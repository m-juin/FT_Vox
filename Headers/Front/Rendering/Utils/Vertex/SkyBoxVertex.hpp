#ifndef __SKYBOX_VERTEX_HPP__
#define __SKYBOX_VERTEX_HPP__

#include "MathGraphicalLib/Vectors/Defines.hpp"

#include <vulkan/vulkan.h>
#include <array>

#include <cstdint>

namespace Vox::Front::Rendering::Utils::Vertex
{
	using namespace MGL::Vectors::Types;
	struct SkyBoxVertex
	{
			Vector3Float vertPos;

			SkyBoxVertex(Vector3Float vertPos_) : vertPos(vertPos_) {};

			bool operator==(const SkyBoxVertex &v1)
			{
				return (this->vertPos == v1.vertPos);
			}

			static VkVertexInputBindingDescription GetBindingDescription()
			{
				VkVertexInputBindingDescription bindingDescription{};
				bindingDescription.binding = 0;
				bindingDescription.stride = sizeof(SkyBoxVertex);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return bindingDescription;
			}

			static std::array<VkVertexInputAttributeDescription, 1> GetAttributeDescriptions()
			{
				std::array<VkVertexInputAttributeDescription, 1> attributeDescriptions{};
				attributeDescriptions[0].binding = 0;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(SkyBoxVertex, vertPos);

				return attributeDescriptions;
			}
	};

} // namespace Vox::Front::Rendering::Utils::Vertex

#endif // __SKYBOX_VERTEX_HPP__