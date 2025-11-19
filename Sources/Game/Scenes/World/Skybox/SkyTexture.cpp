#include "Game/Scenes/World/Skybox/SkyTexture.hpp"
#include "Game/GameManager.hpp"

#include "stb/stb_image.h"

#include <array>

#include "Front/Rendering/Images/VulkanImage.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/Utils/Buffers/Utils.hpp"

namespace Vox::Game::World::Skybox
{
	const std::array<const char *, 6> textureList = {
		"Right", "Left", "Top", "Bot", "Front", "Back",
	};

	SkyTexture::SkyTexture(std::string path) : Front::Rendering::Images::VulkanImage(512, 512)
	{
		path = path + "Skybox/";

		this->CheckSBValidity(path);
		std::cout << this->_width << std::endl;

		this->_layerCount = 6;
		this->CreateImage(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
						  VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 1);
		auto val = this->LoadTextures(path);
		auto buffer = this->CreateStagingBuffer(val.first, val.second);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		this->CopyBufferToImage(buffer.first);
		vkDestroyBuffer(Front::Rendering::Device::GetInstance().GetLogicalDevice(), buffer.first, nullptr);
		vkFreeMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), buffer.second, nullptr);
		this->CreateView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_CUBE);
		this->CreateSampler();
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	SkyTexture::~SkyTexture() {}

	void SkyTexture::CheckSBValidity(const std::string &path)
	{
		for (const char *texture : textureList)
		{
			int imgWidth, imgHeight, imgChannels;
			const std::string currentPath = path + texture + ".png";
			std::cout << currentPath << std::endl;
			stbi_uc *imgData = stbi_load(currentPath.c_str(), &imgWidth, &imgHeight, &imgChannels, 4);
			if (imgData != nullptr)
			{
				std::cout << "I-I\n";
				this->_width = imgWidth;
				this->_height = imgHeight;
				stbi_image_free(imgData);
			}
		}
	}

	void SkyTexture::CreateImage(VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
								 VkMemoryPropertyFlags properties, uint32_t mipLevels)
	{
		Front::Rendering::Device &device = Front::Rendering::Device::GetInstance();
		this->_mipLevels = mipLevels;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent = {_width, _height, 1};
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = 6;
		imageInfo.format = format;
		imageInfo.tiling = tiling;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = usage;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		if (vkCreateImage(device.GetLogicalDevice(), &imageInfo, nullptr, &this->_image) != VK_SUCCESS)
			throw std::runtime_error("failed to create image!");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device.GetLogicalDevice(), this->_image, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = Front::Rendering::Utils::Buffers::Utils::FindMemoryType(
			memRequirements.memoryTypeBits, properties, device.GetPhysicalDevice());

		if (vkAllocateMemory(device.GetLogicalDevice(), &allocInfo, nullptr, &this->_memory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate image memory!");
		}

		vkBindImageMemory(device.GetLogicalDevice(), this->_image, this->_memory, 0);
	}

	void SkyTexture::CopyBufferToImage(VkBuffer buffer)
	{
		VkCommandBuffer commandBuffer = Front::Rendering::Utils::Buffers::Utils::BeginSingleTimeCommands(
			Front::Rendering::Device::GetInstance().GetLogicalDevice(),
			Front::Rendering::CommandsPool::GetInstance().GetPool());

		std::vector<VkBufferImageCopy> regions;

		for (uint32_t face = 0; face < 6; face++)
		{
			VkBufferImageCopy region{};
			region.bufferOffset = (this->_height * this->_width * 4) * face;
			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = face;
			region.imageSubresource.layerCount = 1;
			region.imageExtent = {_width, _height, 1};
			regions.push_back(region);
		}

		vkCmdCopyBufferToImage(commandBuffer, buffer, this->_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
							   regions.size(), regions.data());

		Front::Rendering::Utils::Buffers::Utils::EndSingleTimeCommands(
			commandBuffer, Front::Rendering::Device::GetInstance().GetLogicalDevice(),
			Front::Rendering::CommandsPool::GetInstance().GetPool(),
			Front::Rendering::SwapChain::GetInstance().GetGraphicQueue());
	}

	void SkyTexture::CreateSampler()
	{
		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(Front::Rendering::Device::GetInstance().GetPhysicalDevice(), &properties);

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = VK_LOD_CLAMP_NONE;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		if (vkCreateSampler(Front::Rendering::Device::GetInstance().GetLogicalDevice(), &samplerInfo, nullptr,
							&this->_sampler) != VK_SUCCESS)
			throw std::runtime_error("Failed to create texture sampler!");
	}

	std::pair<std::array<unsigned char *, 6>, VkDeviceSize> SkyTexture::LoadTextures(const std::string &path)
	{
		std::array<unsigned char *, 6> ret;

		VkDeviceSize size = 0;
		for (size_t count = 0; count < 6; count++)
		{
			const std::string current = path + textureList[count] + ".png";
			int imgWidth, imgHeight, imgChannels;
			unsigned char *imgData = stbi_load(current.c_str(), &imgWidth, &imgHeight, &imgChannels, 4);
			if (count == 0)
				size = imgWidth * imgHeight * 4;
			else if (imgWidth * imgHeight * 4 != static_cast<int>(size))
			{
				stbi_image_free(imgData);
				for (size_t j = 0; j < count; ++j)
					delete[] ret[j];
			}

			ret[count] = new unsigned char[size];
			memcpy(ret[count], imgData, static_cast<size_t>(size));
			stbi_image_free(imgData);
		}
		return {ret, size * 6};
	}

	std::pair<VkBuffer, VkDeviceMemory> SkyTexture::CreateStagingBuffer(std::array<unsigned char *, 6> array,
																		VkDeviceSize size)
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Front::Rendering::Utils::Buffers::Utils::CreateBuffer(
			Front::Rendering::Device::GetInstance().GetLogicalDevice(),
			Front::Rendering::Device::GetInstance().GetPhysicalDevice(), size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
			stagingBufferMemory);

		void *data = nullptr;
		vkMapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, 0, size, 0, &data);
		unsigned char *dst = static_cast<unsigned char *>(data);
		VkDeviceSize faceSize = size / 6;

		for (size_t count = 0; count < 6; count++)
		{
			memcpy(dst + (count * faceSize), array[count], static_cast<size_t>(faceSize));
		}
		vkUnmapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory);

		return {stagingBuffer, stagingBufferMemory};
	}
} // namespace Vox::Game::World::Skybox