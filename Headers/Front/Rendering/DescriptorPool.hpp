#ifndef __DESCRIPTORPOOL_HPP__
#define __DESCRIPTORPOOL_HPP__

#include <vulkan/vulkan.h>

namespace Vox
{
	namespace Front
	{
		namespace Rendering
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
                    VkDescriptorPool &GetPool() {return this->_pool;};
                    DescriptorPool(Device *device);
                    ~DescriptorPool();
            
            };  
        }
    }
}
#endif // __DESCRIPTORPOOL_HPP__