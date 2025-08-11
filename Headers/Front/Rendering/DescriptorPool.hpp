#ifndef __DESCRIPTORPOOL_HPP__
#define __DESCRIPTORPOOL_HPP__

#include <vulkan/vulkan.h>

namespace Vox::Front::Rendering
{
	class Device;

	class DescriptorPool
	{
		private:
			VkDescriptorPool _pool;
			Device *_device;
			/* private */

			DescriptorPool();

		public:
			VkDescriptorPool &GetPool()
			{
				return this->_pool;
			};
			DescriptorPool(Device *device);
			~DescriptorPool();
	};
} // namespace Vox::Front::Rendering
#endif // __DESCRIPTORPOOL_HPP__