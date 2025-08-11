#ifndef __VALIDATIONLAYERS_HPP__
#define __VALIDATIONLAYERS_HPP__

#include <cstring>
#include <vector>

namespace Vox::Front::Rendering
{
	const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
	bool CheckValidationLayerSupport();
} // namespace Vox::Front::Rendering

#endif // __VALIDATIONLAYERS_HPP__