#include "Front/Rendering/ValidationLayers.hpp"
#include <vulkan/vulkan.h>

namespace SplineDesigner
{
	namespace Front
	{
		namespace Rendering
		{
			bool CheckValidationLayerSupport()
			{
				uint32_t layerCount;
				vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
				std::vector<VkLayerProperties> availableLayers(layerCount);
				vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
				for (const char *layerName : validationLayers)
				{
					bool layerFound = false;
					for (const auto &layerProperties : availableLayers)
					{
						if (strcmp(layerName, layerProperties.layerName) == 0)
						{
							layerFound = true;
							break;
						}
					}
					if (!layerFound)
						return false;
				}
				return true;
			}
		} // namespace Rendering
	} // namespace Front
} // namespace SplineDesigner