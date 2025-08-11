#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/PipelineManager.hpp"
#include "Front/Rendering/SwapChain.hpp"

#include "Front/Rendering/QueueFamilyIndices.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include <array>
#include <stdexcept>

namespace Vox::Front::Rendering
{
	void CommandsPool::CreateCommandBuffer()
	{
		this->_buffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = this->_pool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)this->_buffers.size();

		if (vkAllocateCommandBuffers(Device::GetInstance().GetLogicalDevice(), &allocInfo, this->_buffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate command buffers!");
	}

	void CommandsPool::ResetBuffer(uint32_t bufferIndex)
	{
		vkResetCommandBuffer(this->_buffers[bufferIndex], 0);
	}

	void CommandsPool::BeginRecord(uint32_t imageIndex, uint32_t frame)
	{
		Front::Rendering::SwapChain &swap = SwapChain::GetInstance();
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(this->_buffers[frame], &beginInfo) != VK_SUCCESS)
			throw std::runtime_error("Failed to begin recording command buffer!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = PipelineManager::GetInstance().GetRenderPass();
		renderPassInfo.framebuffer = swap.GetFrameBuffer(imageIndex);
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = swap.GetExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
		clearValues[1].depthStencil = {1.0f, 0};
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(this->_buffers[frame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)swap.GetExtent().width;
		viewport.height = (float)swap.GetExtent().height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(this->_buffers[frame], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = swap.GetExtent();

		vkCmdSetScissor(this->_buffers[frame], 0, 1, &scissor);
	}

	void CommandsPool::EndRecord(uint32_t frame)
	{
		vkCmdEndRenderPass(this->_buffers[frame]);

		if (vkEndCommandBuffer(this->_buffers[frame]) != VK_SUCCESS)
			throw std::runtime_error("Failed to record command buffer!");
	}

	CommandsPool::CommandsPool(VkSurfaceKHR &surface)
	{
		QueueFamilyIndices queueFamilyIndices =
			QueueFamilyIndices::findQueueFamilies(Device::GetInstance().GetPhysicalDevice(), surface);

		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

		if (vkCreateCommandPool(Device::GetInstance().GetLogicalDevice(), &poolInfo, nullptr, &this->_pool) != VK_SUCCESS)
			throw std::runtime_error("Failed to create command pool!");

	}

	CommandsPool::~CommandsPool() {}
} // namespace Vox::Front::Rendering