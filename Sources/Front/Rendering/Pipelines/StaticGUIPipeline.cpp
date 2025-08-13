#include "Front/Rendering/Pipelines/StaticGUIPipeline.hpp"

#include "Utils/ReadFile.hpp"

namespace Vox::Front::Rendering::Pipelines
{
	StaticGUIPipeline::StaticGUIPipeline() : APipeline() {}

	StaticGUIPipeline::~StaticGUIPipeline() {}

	void StaticGUIPipeline::CreatePipeline(VkDescriptorSetLayout &layout)
	{
		auto vertShaderCode = Utils::Files::ReadFile("Shaders/GUIImage/Shader.vert.spv");
		auto fragShaderCode = Utils::Files::ReadFile("Shaders/GUIImage/Shader.frag.spv");

		VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertShaderModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragShaderModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        
	}
} // namespace Vox::Front::Rendering::Pipelines