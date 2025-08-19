#ifndef __DEPTHIMAGE_HPP__
#define __DEPTHIMAGE_HPP__

#include "VulkanImage.hpp"

namespace Vox::Front::Rendering::Images
{
	class DepthImage : public VulkanImage
	{
		private:

			/* private */

		public:
			DepthImage();
			~DepthImage();
	};
} // namespace Vox::Front::Rendering::Images

#endif // __DEPTHIMAGE_HPP__