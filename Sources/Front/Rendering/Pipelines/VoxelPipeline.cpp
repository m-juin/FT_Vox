#include "Front/Rendering/Pipelines/VoxelPipeline.hpp"

#include "Front/Rendering/DescriptorPool.hpp"
#include "Front/Rendering/Device.hpp"

#include <array>

namespace Vox::Front::Rendering::Pipelines
{
	VoxelPipeline::VoxelPipeline()
	{
		this->CreateSet(DescriptorPool::GetInstance().GetPool());
		this->CreatePipeline();
	}

	VoxelPipeline::~VoxelPipeline() {}

	void VoxelPipeline::CreatePipeline() {}

	void VoxelPipeline::CreateSet(VkDescriptorPool &descPool) 
    {
        this->CreateSetLayout();
        this->_set.resize(2);

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
		allocInfo.pSetLayouts = &this->_slayout;

        if (vkAllocateDescriptorSets(Device::GetInstance().GetLogicalDevice(), &allocInfo, this->_set.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate descriptor sets!");
        
        

    }

	void VoxelPipeline::CreateSetLayout()
	{
		std::array<VkDescriptorSetLayoutBinding, 2> samplerLayoutBindings{};

		samplerLayoutBindings[0].binding = 0;
		samplerLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		samplerLayoutBindings[0].descriptorCount = 1;
		samplerLayoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		samplerLayoutBindings[1].binding = 1;
		samplerLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		samplerLayoutBindings[1].descriptorCount = 1;
		samplerLayoutBindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(samplerLayoutBindings.size());
		layoutInfo.pBindings = samplerLayoutBindings.data();

		if (vkCreateDescriptorSetLayout(Device::GetInstance().GetLogicalDevice(), &layoutInfo, nullptr,
										&this->_slayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create descriptor set layout!");
	}
} // namespace Vox::Front::Rendering::Pipelines