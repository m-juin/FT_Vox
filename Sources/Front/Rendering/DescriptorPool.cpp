
#include "Front/Rendering/DescriptorPool.hpp"

#include "Front/Rendering/Device.hpp"

#include <stdexcept>

#include <array>

namespace Vox::Front::Rendering
{
	DescriptorPool::DescriptorPool()
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes;
		poolSizes[0].descriptorCount = 2;
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Interface, texture atlas + font atlas;
		
		poolSizes[1].descriptorCount = 2;
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC; // VoxelObjects

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();

		size_t total = 3;
		// for (auto info : poolSizes)
		// 	total += info.descriptorCount;

		poolInfo.maxSets = total;

		if (vkCreateDescriptorPool(Device::GetInstance().GetLogicalDevice(), &poolInfo, nullptr, &this->_pool) != VK_SUCCESS)
			throw std::runtime_error("Failed to create descriptor pool!");
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(Device::GetInstance().GetLogicalDevice(), this->_pool, nullptr);
	}
} // namespace Vox::Front::Rendering