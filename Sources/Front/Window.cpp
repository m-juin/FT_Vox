#include "Front/Window.hpp"
#include "Front/Rendering/VulkanManager.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Vox::Front
{
	Window::Window(uint16_t width, uint16_t height)
	{
		if (glfwInit() == false)
		{
			glfwTerminate();
			exit(1);
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		this->_window = glfwCreateWindow(width, height, "FT_Vox", nullptr, nullptr);
		if (this->_window == nullptr)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
	}

	void Window::SetupSurface()
	{
		if (glfwCreateWindowSurface(Front::Rendering::VulkanManager::GetInstance().GetVkInstance(), this->_window, nullptr, &this->_surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create window surface!");
	}

	Window::~Window()
	{
		vkDestroySurfaceKHR(Rendering::VulkanManager::GetInstance().GetVkInstance(), this->_surface, nullptr);
		if (this->_window != nullptr)
			glfwDestroyWindow(this->_window);

		glfwTerminate();
	}
} // namespace Vox::Front