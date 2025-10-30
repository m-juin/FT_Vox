#include "Front/Rendering/Pipelines/StaticGUIPipeline.hpp"

#include "Front/Rendering/Utils/Vertex/StaticGUIVertex.hpp"

#include "Front/Rendering/DescriptorPool.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"

#include "Utils/ReadFile.hpp"

#include "Front/Utils/TexturesAtlas.hpp"

#include "Front/Rendering/Images/FontImage.hpp"

#include "Game/GameManager.hpp"

#include "Front/Utils/TexturesData.hpp"

namespace Vox::Front::Rendering::Pipelines
{
	StaticGUIPipeline::StaticGUIPipeline() : APipeline()
	{
		this->CreateSet(DescriptorPool::GetInstance().GetPool());
		this->CreatePipeline();
	}

	StaticGUIPipeline::~StaticGUIPipeline() {}

	void StaticGUIPipeline::CreatePipeline()
	{
		auto vertShaderCode = Vox::Utils::Files::ReadFile("Shaders/GUIStaticV1/Shader.vert.spv");
		auto fragShaderCode = Vox::Utils::Files::ReadFile("Shaders/GUIStaticV1/Shader.frag.spv");

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

		auto bindingDescription = Utils::Vertex::StaticGUIVertex::GetBindingDescription();
		auto attributeDescriptions = Utils::Vertex::StaticGUIVertex::GetAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_FALSE;
		depthStencil.depthWriteEnable = VK_FALSE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_NEVER;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f;
		depthStencil.maxDepthBounds = 1.0f;
		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {};
		depthStencil.back = {};

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_NONE;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPushConstantRange pushConstRange{};
		pushConstRange.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstRange.offset = 0;
		pushConstRange.size = sizeof(Front::Utils::TexturesAtlas::TexturesAtlas::uvData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = &this->_slayout;

		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstRange;

		auto device = Device::GetInstance().GetLogicalDevice();

		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &this->_layout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = this->_layout;
		pipelineInfo.renderPass = PipelinesManager::GetInstance().GetRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->_instance) !=
			VK_SUCCESS)
			throw std::runtime_error("Failed to create graphics pipeline!");

		vkDestroyShaderModule(device, fragShaderModule, nullptr);
		vkDestroyShaderModule(device, vertShaderModule, nullptr);
	}

	void StaticGUIPipeline::CreateSetLayout()
	{
		std::array<VkDescriptorSetLayoutBinding, 3> samplerLayoutBindings{};
		samplerLayoutBindings[0].binding = 0;
		samplerLayoutBindings[0].descriptorCount = 1;
		samplerLayoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBindings[0].pImmutableSamplers = nullptr;
		samplerLayoutBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		samplerLayoutBindings[1].binding = 1;
		samplerLayoutBindings[1].descriptorCount = 1;
		samplerLayoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBindings[1].pImmutableSamplers = nullptr;
		samplerLayoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		samplerLayoutBindings[2].binding = 2;
		samplerLayoutBindings[2].descriptorCount = Vox::Front::Utils::TexturesData::MAX_DYNAMIC_TEXTURES;
		samplerLayoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBindings[2].pImmutableSamplers = nullptr;
		samplerLayoutBindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		// std::vector<VkDescriptorSetLayoutBinding> bindings = {samplerLayoutBinding};

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(samplerLayoutBindings.size());
		layoutInfo.pBindings = samplerLayoutBindings.data();

		if (vkCreateDescriptorSetLayout(Device::GetInstance().GetLogicalDevice(), &layoutInfo, nullptr,
										&this->_slayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create descriptor set layout!");
	}

	void StaticGUIPipeline::CreateSet(VkDescriptorPool &descPool)
	{
		this->CreateSetLayout();

		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(1);
		allocInfo.pSetLayouts = &this->_slayout;

		this->_set.resize(allocInfo.descriptorSetCount);

		if (vkAllocateDescriptorSets(Device::GetInstance().GetLogicalDevice(), &allocInfo, this->_set.data()) !=
			VK_SUCCESS)
			throw std::runtime_error("Failed to allocate descriptor sets!");
		InitSet();
	}

	void StaticGUIPipeline::UpdateSet(const size_t &index)
	{
		if (this->_slayout == VK_NULL_HANDLE)
			return;
		if (index >= _dynamicInfos.size())
		{
			std::cerr << "UpdateSet index out of range: " << index << std::endl;
			return;
		}

		const auto &dynamicsTextures = Game::GameManager::GetInstance().GetTexturesManager().GetDynamics();
		if (index >= dynamicsTextures.size())
		{
			std::cerr << "Dynamics textures too small\n";
			return;
		}

		std::cout << "\033[1;32m" << "[DEBUG] Dynamic descriptor set at " << index << " Updated" << "\033[0m"
				  << std::endl;

		_dynamicInfos[index].imageView = dynamicsTextures[index];
		_dynamicInfos[index].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		_dynamicInfos[index].sampler = Game::GameManager::GetInstance().GetTexturesManager().GetDynamicSampler();

		VkWriteDescriptorSet write = {};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.pNext = nullptr;
		write.dstSet = this->_set[0];
		write.dstBinding = 2;
		write.dstArrayElement = static_cast<uint32_t>(index);
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		write.descriptorCount = 1;
		write.pImageInfo = &_dynamicInfos[index];

		vkUpdateDescriptorSets(Device::GetInstance().GetLogicalDevice(), 1, &write, 0, nullptr);
	}

	void StaticGUIPipeline::InitSet()
	{
		auto &texturesManager = Game::GameManager::GetInstance().GetTexturesManager();

		auto textureAtlas = texturesManager.operator[]("Menu_Main");
		auto &fontAtlas = texturesManager.GetFont();

		VkDescriptorImageInfo textureInfo{};
		textureInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		textureInfo.imageView = textureAtlas->_mainAtlas->GetView();
		textureInfo.sampler = textureAtlas->_mainAtlas->GetSampler();

		VkDescriptorImageInfo fontInfo{};
		fontInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		fontInfo.imageView = fontAtlas.GetView();
		fontInfo.sampler = fontAtlas.GetSampler();

		// std::array<VkWriteDescriptorSet, 3> descriptorWrites{};
		for (auto &desc : _descriptorWrites)
			desc = {};
		_descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		_descriptorWrites[0].dstSet = this->_set[0];
		_descriptorWrites[0].dstBinding = 0;
		_descriptorWrites[0].dstArrayElement = 0;
		_descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		_descriptorWrites[0].descriptorCount = 1;
		_descriptorWrites[0].pImageInfo = &textureInfo;

		_descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		_descriptorWrites[1].dstSet = this->_set[0];
		_descriptorWrites[1].dstBinding = 1;
		_descriptorWrites[1].dstArrayElement = 0;
		_descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		_descriptorWrites[1].descriptorCount = 1;
		_descriptorWrites[1].pImageInfo = &fontInfo;

		const auto &dynamicsTextures = texturesManager.GetDynamics();
		const auto &sampler = texturesManager.GetDynamicSampler();

		for (size_t index = 0; index < Vox::Front::Utils::TexturesData::MAX_DYNAMIC_TEXTURES; index++)
		{
			_dynamicInfos[index].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			_dynamicInfos[index].imageView = dynamicsTextures[index];
			_dynamicInfos[index].sampler = sampler;
		}
		_descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		_descriptorWrites[2].dstSet = this->_set[0];
		_descriptorWrites[2].dstBinding = 2;
		_descriptorWrites[2].dstArrayElement = 0;
		_descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		_descriptorWrites[2].descriptorCount = Vox::Front::Utils::TexturesData::MAX_DYNAMIC_TEXTURES;
		_descriptorWrites[2].pImageInfo = _dynamicInfos.data();

		vkUpdateDescriptorSets(Device::GetInstance().GetLogicalDevice(),
							   static_cast<uint32_t>(_descriptorWrites.size()), _descriptorWrites.data(), 0, nullptr);
	}
} // namespace Vox::Front::Rendering::Pipelines