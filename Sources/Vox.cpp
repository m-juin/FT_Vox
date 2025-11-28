#include <iostream>

#include "Utils/Colors.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/DescriptorPool.hpp"
#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/SyncObjects.hpp"
#include "Front/Rendering/VulkanManager.hpp"
#include "Front/Window.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"

#include "Front/Rendering/Images/DepthImage.hpp"
#include "Game/GameManager.hpp"

#include <GLFW/glfw3.h>

#include "LoggerLib/UtilityFunctions.hpp"

using namespace Vox::Front;

#include "Front/Utils/TexturesAtlas.hpp"

#ifdef TRACY_ENABLE
#include "Utils/TracyUtils.hpp"
#endif

void CleanUp()
{
#ifdef TRACY_ENABLE
	Vox::TracyUtils::cleanupVulkanTracy();
#endif
	Vox::Game::GameManager::Clean();
	Interfaces::InterfacesManager::Clean();

	Rendering::SwapChain::Clean();
	Rendering::Pipelines::PipelinesManager::Clean();
	Rendering::DescriptorPool::Clean();
	Rendering::SyncObjects::Clean();
	Rendering::CommandsPool::Clean();
	delete Rendering::VulkanManager::GetInstance().GetDepthImage();
	Rendering::Device::Clean();

	Window::Clean();
	Rendering::VulkanManager::Clean();
}

int main()
{
	LoggerLib::Logger::InitLogger(LoggerLib::LogLevel::NONE, std::cout, {true, true, false, true});

	Window::Init(1920, 1080);
	Rendering::VulkanManager::Init();
	Window::GetInstance().SetupSurface();

	Rendering::Device::Init(Rendering::VulkanManager::GetInstance().GetVkInstance(),
							Window::GetInstance().GetSurface());

	Rendering::Device &device = Rendering::Device::GetInstance();

	Rendering::SwapChain::Init();
	Rendering::DescriptorPool::Init();
	Rendering::Pipelines::PipelinesManager::Init();
	Rendering::VulkanManager::GetInstance().SetDepthImage(new Rendering::Images::DepthImage());
	Rendering::SwapChain::GetInstance().CreateFrameBuffer(
		Rendering::Pipelines::PipelinesManager::GetInstance().GetRenderPass(),
		Rendering::VulkanManager::GetInstance().GetDepthImage()->GetView());
	Rendering::CommandsPool::Init(Window::GetInstance().GetSurface());
	Rendering::SyncObjects::Init();

	Rendering::CommandsPool &pool = Rendering::CommandsPool::GetInstance();
	Rendering::SyncObjects &sync = Rendering::SyncObjects::GetInstance();
	Rendering::SwapChain &swap = Rendering::SwapChain::GetInstance();

	pool.CreateCommandBuffer();

	Vox::Game::GameManager::Init();
	Interfaces::InterfacesManager::Init();

	auto &gm = Vox::Game::GameManager::GetInstance();
	Rendering::Pipelines::PipelinesManager::GetInstance().CreatePipelines();
#ifdef TRACY_ENABLE
	Vox::TracyUtils::initVulkanTracy(device.GetLogicalDevice(), device.GetPhysicalDevice(),
									 Rendering::SwapChain::GetInstance().GetGraphicQueue(),
									 Rendering::CommandsPool::GetInstance().GetBuffer(0));
#endif
	gm.InitGame();

	while (!glfwWindowShouldClose(Window::GetInstance().GetWindow()))
	{
		// ZoneScoped;
// #ifdef TRACY_ENABLE
// 		ZoneScoped;
// #endif
		VkFence fence = sync.GetCurrentFence();
		uint32_t currentFrame = sync.GetCurrentFrame();
		vkWaitForFences(device.GetLogicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX);

		uint32_t imageIndex = 0;
		VkResult result = vkAcquireNextImageKHR(device.GetLogicalDevice(), swap.GetVulkanInstance(), UINT64_MAX,
												sync.GetCurrentImageSemaphore(), VK_NULL_HANDLE, &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			swap.RecreateSwapChain();
			sync.GoToNextFrame();
			continue;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire swap chain image!");

		vkResetFences(device.GetLogicalDevice(), 1, &fence);
		gm.Update();
		pool.ResetBuffer(currentFrame);
		pool.BeginRecord(imageIndex, currentFrame);
		gm.Render();
#ifdef TRACY_ENABLE
		FrameMark; // Marque la fin de la frame
#endif
		pool.EndRecord(currentFrame);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = {sync.GetCurrentImageSemaphore()};
		VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &pool.GetBuffer(currentFrame);

		VkSemaphore signalSemaphores[] = {sync.GetCurrentRenderFinishedSemaphore()};
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		if (vkQueueSubmit(swap.GetGraphicQueue(), 1, &submitInfo, fence) != VK_SUCCESS)
			throw std::runtime_error("Failed to submit draw command buffer!");

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = {swap.GetVulkanInstance()};
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(swap.GetPresentQueue(), &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) // || FrameBufferResized
		{
			swap.RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to present swap chain image!");

		sync.GoToNextFrame();
		glfwPollEvents();
		gm.GetSceneManager().ProcessSceneChange();
	}
	vkDeviceWaitIdle(device.GetLogicalDevice());

	CleanUp();
}