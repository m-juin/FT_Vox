#ifndef __DESCRIPTORPOOL_HPP__
#define __DESCRIPTORPOOL_HPP__

#include "Utils/Singleton.hpp"
#include <vulkan/vulkan.h>

namespace Vox::Front::Rendering
{
	class Device;

	class DescriptorPool : public Vox::Utils::Singleton<DescriptorPool>
	{
		friend class Vox::Utils::Singleton<DescriptorPool>;
		private:
			VkDescriptorPool _pool;
			/* private */

			DescriptorPool();

		public:
			VkDescriptorPool &GetPool()
			{
				return this->_pool;
			};
			~DescriptorPool();
	};
} // namespace Vox::Front::Rendering
#endif // __DESCRIPTORPOOL_HPP__