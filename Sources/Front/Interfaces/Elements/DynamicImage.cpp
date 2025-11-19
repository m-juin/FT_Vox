#include "Front/Interfaces/Elements/DynamicImage.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/StaticGUIPipeline.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/SyncObjects.hpp"
#include "Front/Rendering/Device.hpp"

#include "Front/Rendering/Utils/Buffers/Utils.hpp"

#include <cstring>

#include "Front/Scenes/TexturesManager.hpp"

namespace Vox::Front::Interfaces::Elements
{
	DynamicImage::DynamicImage(const Constructor &st) : AElement(st.pos, st.size), VulkanImage(400, 400), _imgData(st.defaultData), _colorMod(st.colorMod)
	{
		this->ResetVertex();
		B_Vertices =
			new dbuffer(1, 4 * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
		B_Indices =
			new dbuffer(1, 6 * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

		B_Vertices->Create(&this->_vertex);

		uint16_t *indices = new uint16_t[6]{0, 1, 2, 2, 3, 0};

		B_Indices->Create(indices);

		delete[] indices;
		this->CreateVulkanImage();
	}

	void DynamicImage::SetPos(const Vector2 newPos)
	{
		if (this->_pos == newPos)
			return;
		this->_pos = newPos;
	}

	void DynamicImage::SetSize(const Vector2 newSize)
	{
		if (this->_size == newSize)
			return;
		this->_size = newSize;
	}

	void DynamicImage::SetData(std::vector<uint8_t> newData)
	{
		this->_imgData = newData;
		this->UpdateVulkanImage();
	}

	void DynamicImage::ResetVertex()
	{
		const Vector2 screenSize(Rendering::SwapChain::GetInstance().GetExtent().width,
								 Rendering::SwapChain::GetInstance().GetExtent().height);

		this->_uvMappingData.uvMin[0] = 0.0f;
		this->_uvMappingData.uvMin[1] = 0.0f;
		this->_uvMappingData.uvMax[0] = 1.0f;
		this->_uvMappingData.uvMax[1] = 1.0f;
		this->_uvMappingData.textureID = -1;

		this->_vertex[0] = Vertex(PointPixelToVulkan(this->_pos, screenSize), {0.0f, 0.0f}, this->_colorMod, Rendering::Utils::Vertex::E_ImageType::DynamicTexture);
		this->_vertex[1] = Vertex(PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1]}, screenSize),
								  {1.0f, 0.0f}, this->_colorMod, Rendering::Utils::Vertex::E_ImageType::DynamicTexture);
		this->_vertex[2] =
			Vertex(PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1] + this->_size[1]}, screenSize),
				   {1.0f, 1.0f}, this->_colorMod, Rendering::Utils::Vertex::E_ImageType::DynamicTexture);
		this->_vertex[3] = Vertex(PointPixelToVulkan({this->_pos[0], this->_pos[1] + this->_size[1]}, screenSize),
								  {0.0f, 1.0f}, this->_colorMod, Rendering::Utils::Vertex::E_ImageType::DynamicTexture);
	}

	DynamicImage::~DynamicImage()
	{
		if (B_Vertices)
			delete B_Vertices;
		if (B_Indices)
			delete B_Indices;
		if (this->_uvMappingData.textureID != -1)
			Game::GameManager::GetInstance().GetTexturesManager().RemoveDynamicImage(this->_uvMappingData.textureID);
	}

	void DynamicImage::CreateVulkanImage()
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Front::Rendering::Utils::Buffers::Utils::CreateBuffer(
			Front::Rendering::Device::GetInstance().GetLogicalDevice(),
			Front::Rendering::Device::GetInstance().GetPhysicalDevice(), this->_imgData.size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
			stagingBufferMemory);

		void *data;
		vkMapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, 0,
					this->_imgData.size(), 0, &data);
		memcpy(data, this->_imgData.data(), this->_imgData.size());
		vkUnmapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory);

		this->CreateImage(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
						  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
						  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		this->CopyBufferToImage(stagingBuffer);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, nullptr);

		this->CreateView(VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT);

		this->_uvMappingData.textureID =
			Game::GameManager::GetInstance().GetTexturesManager().AddDynamicImage(this->_view);
			std::cout << this->_uvMappingData.textureID << std::endl;
	}

	void DynamicImage::Draw()
	{
		auto cmdBuffer =
			Rendering::CommandsPool::GetInstance().GetBuffer(Rendering::SyncObjects::GetInstance().GetCurrentFrame());
		VkDeviceSize offsets[] = {0};

		vkCmdPushConstants(cmdBuffer,
						   Front::Rendering::Pipelines::PipelinesManager::GetInstance()
							   .operator[]<Front::Rendering::Pipelines::StaticGUIPipeline>("StaticGUI")
							   ->GetLayout(),
						   VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(this->_uvMappingData), &this->_uvMappingData);
		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &this->B_Vertices->GetBuffer(0), offsets);
		vkCmdBindIndexBuffer(cmdBuffer, this->B_Indices->GetBuffer(0), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);
	}

	void DynamicImage::UpdateVulkanImage()
	{
		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		Front::Rendering::Utils::Buffers::Utils::CreateBuffer(
			Front::Rendering::Device::GetInstance().GetLogicalDevice(),
			Front::Rendering::Device::GetInstance().GetPhysicalDevice(), this->_imgData.size(),
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
			stagingBufferMemory);

		void *data;
		vkMapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, 0,
					this->_imgData.size(), 0, &data);
		memcpy(data, this->_imgData.data(), this->_imgData.size());
		vkUnmapMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		this->CopyBufferToImage(stagingBuffer);
		this->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		vkDestroyBuffer(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(Front::Rendering::Device::GetInstance().GetLogicalDevice(), stagingBufferMemory, nullptr);
	}
} // namespace Vox::Front::Interfaces::Elements