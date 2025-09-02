#ifndef __VULKAN_HPP__
#define __VULKAN_HPP__

#include <vulkan/vulkan.h>

#include "Front/Rendering/Device.hpp"
#include "../Chuncks/VoxelChunck.hpp"


namespace Vox::Game::Utils::Vulkan
{
	inline size_t GetAlignedChunckSize()
	{
        VkPhysicalDevice device = Front::Rendering::Device::GetInstance().GetPhysicalDevice();

		VkPhysicalDeviceProperties props{};
		vkGetPhysicalDeviceProperties(device, &props);

		size_t minAlignment = props.limits.minUniformBufferOffsetAlignment;

		size_t chunkSize = sizeof(World::Chuncks::VoxelChunck::ChunckUniform);
		size_t alignedChunkSize = (chunkSize + minAlignment - 1) & ~(minAlignment - 1);
	
        return alignedChunkSize;
    };
} // namespace Vox::Game::Utils::Vulkan

#endif // __VULKAN_HPP__