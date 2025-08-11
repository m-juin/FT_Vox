#include "Front/Rendering/SyncObjects.hpp"

#include "Front/Rendering/Device.hpp"

#include <stdexcept>

namespace Vox::Front::Rendering
{
	void SyncObjects::GoToNextFrame()
	{
		this->_currentFrame = (this->_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	SyncObjects::SyncObjects(Device *device)
	{
		this->_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		this->_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		this->_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(device->GetLogicalDevice(), &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) !=
					VK_SUCCESS ||
				vkCreateSemaphore(device->GetLogicalDevice(), &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) !=
					VK_SUCCESS ||
				vkCreateFence(device->GetLogicalDevice(), &fenceInfo, nullptr, &_inFlightFences[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to create synchronization objects for a frame!");
		}
		this->_device = device;
	}

	SyncObjects::~SyncObjects() {}
} // namespace Vox::Front::Rendering