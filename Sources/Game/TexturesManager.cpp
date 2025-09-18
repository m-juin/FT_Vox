#include "Front/Scenes/TexturesManager.hpp"

#include "Front/Rendering/Device.hpp"

#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/StaticGUIPipeline.hpp"

#include "Front/Rendering/Images/DummyImage.hpp"

namespace Vox::Front::Scenes
{
	TexturesManager::TexturesManager(const std::string &T_Path)
	{
		this->CreateMap(T_Path);
		this->CreateDynamicSampler();

		this->_avalaibleDynamicImages.flip();
		if (Rendering::Images::DummyImage::IsInit() == false)
			Rendering::Images::DummyImage::Init();
		auto &dummy = Rendering::Images::DummyImage::GetInstance(); 
		for (size_t i = 0; i < Vox::Front::Utils::TexturesData::MAX_DYNAMIC_TEXTURES; i++)
			this->_dynamicImages[i] = dummy.GetView();
	}

	TexturesManager::~TexturesManager() {}

	void TexturesManager::CreateMap(const std::string &T_Path)
	{
		this->_texturesMap["Menu_Main"] = new Front::Utils::TexturesAtlas(
			{
				{"Dirt", T_Path + "GUI/dirt.png"},
				{"Button", T_Path + "GUI/button.png"},
				{"Button_Hover", T_Path + "GUI/button_highlighted.png"},
				{"Button_Disabled", T_Path + "GUI/button_disabled.png"},
			},
			4);
		this->_fontImage = std::make_unique<Front::Rendering::Images::FontImage>(T_Path + "GUI/Fonts/Minecraft.ttf");
	}

	int TexturesManager::AddDynamicImage(VkImageView &view)
	{
		int index = this->_avalaibleDynamicImages._Find_first();
		std::cout << "index = " << index << std::endl;
		if (index == Vox::Front::Utils::TexturesData::MAX_DYNAMIC_TEXTURES)
			return -1;
		this->_avalaibleDynamicImages.flip(index);
		this->_dynamicImages[index] = view;
		auto &pipelineManager = Rendering::Pipelines::PipelinesManager::GetInstance();
		auto pipeline = pipelineManager.operator[]<Rendering::Pipelines::StaticGUIPipeline>("tt");

		if (pipeline == nullptr)
			return index;
		pipeline->UpdateSet(index);
		return index;
	}

	void TexturesManager::RemoveDynamicImage(const size_t &index)
	{
		if (!this->_avalaibleDynamicImages[index])
			return;
		this->_avalaibleDynamicImages.flip(index);
		this->_dynamicImages[index] = Rendering::Images::DummyImage::GetInstance().GetView();
	}

	void TexturesManager::CreateDynamicSampler()
	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(Rendering::Device::GetInstance().GetPhysicalDevice(), &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

		if (vkCreateSampler(Rendering::Device::GetInstance().GetLogicalDevice(), &samplerInfo, nullptr,
							&this->_dynamicSampler) != VK_SUCCESS)
			throw std::runtime_error("Failed to create texture sampler!");
	}

} // namespace Vox::Front::Scenes
