
#include "Front/Rendering/DescriptorPool.hpp"

#include "Front/Rendering/Device.hpp"

#include <stdexcept>

namespace Vox::Front::Rendering
{
	DescriptorPool::DescriptorPool()
	{
		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(0);
		poolInfo.pPoolSizes = nullptr;
		poolInfo.maxSets = 0;

		if (vkCreateDescriptorPool(Device::GetInstance().GetLogicalDevice(), &poolInfo, nullptr, &this->_pool) != VK_SUCCESS)
			throw std::runtime_error("Failed to create descriptor pool!");
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(Device::GetInstance().GetLogicalDevice(), this->_pool, nullptr);
	}
} // namespace Vox::Front::Rendering