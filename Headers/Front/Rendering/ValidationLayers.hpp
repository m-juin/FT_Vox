#ifndef __VALIDATIONLAYERS_HPP__
#define __VALIDATIONLAYERS_HPP__

#include <cstring>
#include <vector>

namespace SplineDesigner
{
	namespace Front
	{
		namespace Rendering
		{
			const std::vector<const char *> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
			};
			bool CheckValidationLayerSupport();
		} // namespace Rendering

	} // namespace Front

} // namespace SplineDesigner

#endif // __VALIDATIONLAYERS_HPP__