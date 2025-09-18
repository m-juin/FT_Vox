#include "Front/Rendering/Images/DummyImage.hpp"

#include "Front/Rendering/Device.hpp"

namespace Vox::Front::Rendering::Images
{
	DummyImage::DummyImage() : VulkanImage(1, 1)
	{
		this->CreateImage(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
						  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        this->CreateView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

        this->CreateSampler();
        this->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        this->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	DummyImage::~DummyImage()
    {

    }
} // namespace Vox::Front::Rendering::Images