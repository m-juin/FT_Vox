#include "Front/Rendering/Pipelines/APipeline.hpp"

#include "Front/Rendering/Device.hpp"

namespace Vox::Front::Rendering::Pipelines
{
	APipeline::~APipeline()
	{
		auto device = Device::GetInstance().GetLogicalDevice();

		vkDestroyDescriptorSetLayout(device, this->_slayout, nullptr);
		vkDestroyPipeline(device, this->_instance, nullptr);
		vkDestroyPipelineLayout(device, this->_layout, nullptr);
	}

	VkShaderModule APipeline::CreateShaderModule(const std::vector<char> &code)
	{
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(Device::GetInstance().GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			throw std::runtime_error("Failed to create shader module!");

		return shaderModule;
	}

	APipeline::APipeline()
	{
		for (auto &set : this->_set)
			set = VK_NULL_HANDLE;
		this->_slayout = VK_NULL_HANDLE;
	}
} // namespace Vox::Front::Rendering::Pipelines