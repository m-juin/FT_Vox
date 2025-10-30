
#include "Front/Rendering/DescriptorPool.hpp"

#include "Front/Rendering/Device.hpp"

#include <stdexcept>

#include <array>

#include "Front/Utils/TexturesData.hpp"

namespace Vox::Front::Rendering
{
	DescriptorPool::DescriptorPool()
	{
		std::array<VkDescriptorPoolSize, 9> poolSizes;
		poolSizes[0].descriptorCount = 1;
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // Interface, texture atlas + font atlas;

		poolSizes[1].descriptorCount = 2;
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC; // VoxelObjects

		poolSizes[2].descriptorCount = Vox::Front::Utils::TexturesData::MAX_DYNAMIC_TEXTURES;
		poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // dynamic textures;

		poolSizes[3].descriptorCount = 2;
		poolSizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // BlocksTextures
		
		poolSizes[4].descriptorCount = 2;
		poolSizes[4].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // BlocksTexturesMask

		poolSizes[5].descriptorCount = 2;
		poolSizes[5].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC; // VoxelObjects

		poolSizes[6].descriptorCount = 2;
		poolSizes[6].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // BlocksTexturesTransparent
		
		poolSizes[7].descriptorCount = 2;
		poolSizes[7].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // BlocksTexturesMaskTransparent

		poolSizes[8].descriptorCount = 1;
		poolSizes[8].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER; // BlocksTextures

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();

		size_t total = 20;

		poolInfo.maxSets = total;

		if (vkCreateDescriptorPool(Device::GetInstance().GetLogicalDevice(), &poolInfo, nullptr, &this->_pool) != VK_SUCCESS)
			throw std::runtime_error("Failed to create descriptor pool!");
	}

	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(Device::GetInstance().GetLogicalDevice(), this->_pool, nullptr);
	}
} // namespace Vox::Front::Rendering