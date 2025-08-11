#include <iostream>

#include "Utils/Colors.hpp"

using namespace Vox;

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/DescriptorPool.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/PipelineManager.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/SyncObjects.hpp"
#include "Front/Rendering/VulkanManager.hpp"
#include "Front/Window.hpp"

#include "Front/Rendering/Images/DepthImage.hpp"

#include <GLFW/glfw3.h>

Front::Window *win;
Front::Rendering::VulkanManager *vkManager;

int main()
{
	win = new Front::Window(1920, 1080);
	vkManager = new Front::Rendering::VulkanManager();
	win->SetupSurface(vkManager->GetVkInstance());

	vkManager->SetDevice(new Front::Rendering::Device(vkManager->GetVkInstance(), win->GetSurface()));
	vkManager->SetSwapChain(new Front::Rendering::SwapChain(vkManager->GetDevice(), win));
	vkManager->SetDescPool(new Front::Rendering::DescriptorPool(vkManager->GetDevice()));
	vkManager->SetPipelineManager(
		new Front::Rendering::PipelineManager(vkManager->GetSwapChain(), vkManager->GetDevice()));
	vkManager->SetDepthImage(
		new Front::Rendering::Images::DepthImage(vkManager->GetDevice(), vkManager->GetSwapChain()));
	vkManager->GetSwapChain()->CreateFrameBuffer(vkManager->GetPipelineManager()->GetRenderPass(),
												 vkManager->GetDepthImage()->GetView());
	vkManager->SetCommandsPool(new Front::Rendering::CommandsPool(vkManager->GetDevice(), win->GetSurface()));
	vkManager->SetSyncObjects(new Front::Rendering::SyncObjects(vkManager->GetDevice()));
	vkManager->GetCommandsPool()->CreateCommandBuffer();

	while (!glfwWindowShouldClose(win->GetWindow()))
	{
		VkFence fence = vkManager->GetSyncObjects()->GetCurrentFence();
		uint32_t currentFrame = vkManager->GetSyncObjects()->GetCurrentFrame();
		vkWaitForFences(vkManager->GetDevice()->GetLogicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

		uint32_t imageIndex = 0;
		VkResult result = vkAcquireNextImageKHR(
			vkManager->GetDevice()->GetLogicalDevice(), vkManager->GetSwapChain()->GetVulkanInstance(), UINT64_MAX,
			vkManager->GetSyncObjects()->GetCurrentImageSemaphore(), VK_NULL_HANDLE, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			// TODO Recreate SwapCain.
			vkManager->GetSyncObjects()->GoToNextFrame();
			continue;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire swap chain image!");

		vkResetFences(vkManager->GetDevice()->GetLogicalDevice(), 1, &fence);

		vkManager->GetCommandsPool()->ResetBuffer(currentFrame);
		vkManager->GetCommandsPool()->BeginRecord(imageIndex, currentFrame, vkManager->GetSwapChain(),
												  vkManager->GetPipelineManager());
		// Here goes all render code;
		vkManager->GetCommandsPool()->EndRecord(currentFrame);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {vkManager->GetSyncObjects()->GetCurrentImageSemaphore()};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &vkManager->GetCommandsPool()->GetBuffer(currentFrame);
		;

		VkSemaphore signalSemaphores[] = {vkManager->GetSyncObjects()->GetCurrentRenderFinishedSemaphore()};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		if (vkQueueSubmit(vkManager->GetSwapChain()->GetGraphicQueue(), 1, &submitInfo, fence) != VK_SUCCESS)
			throw std::runtime_error("Failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {vkManager->GetSwapChain()->GetVulkanInstance()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(vkManager->GetSwapChain()->GetPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) // || FrameBufferResized
		{
			// this->ChangeFBStatus(false);
			// SwapChain::GetInstance().Recreate(PipelineManager::GetInstance().GetRenderPass());
		}
		else if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to present swap chain image!");

		vkManager->GetSyncObjects()->GoToNextFrame();
		glfwPollEvents();
	}
	vkDeviceWaitIdle(vkManager->GetDevice()->GetLogicalDevice());
}