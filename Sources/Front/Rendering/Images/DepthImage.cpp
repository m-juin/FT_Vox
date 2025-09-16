#include "Front/Rendering/Images/DepthImage.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/SwapChain.hpp"

#include "Front/Rendering/Utils/Buffers/Utils.hpp"
#include "Front/Rendering/Utils/FindFormat.hpp"

#include <stdexcept>

namespace Vox::Front::Rendering::Images
{
	DepthImage::DepthImage() : VulkanImage(SwapChain::GetInstance().GetExtent().width, SwapChain::GetInstance().GetExtent().height)
	{
		VkFormat format = Utils::FindDepthFormat(Device::GetInstance().GetPhysicalDevice());
		this->CreateImage(format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		this->CreateView(format, VK_IMAGE_ASPECT_DEPTH_BIT);
	}

	DepthImage::~DepthImage()
	{
	}

} // namespace Vox::Front::Rendering::Images