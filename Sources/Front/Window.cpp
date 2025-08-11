#include "Front/Window.hpp"

#include <iostream>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Vox
{
	namespace Front
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

		void Window::SetupSurface(VkInstance &instance)
		{
			if (glfwCreateWindowSurface(instance, this->_window, nullptr, &this->_surface) != VK_SUCCESS)
				throw std::runtime_error("Failed to create window surface!");
		}

		Window::~Window()
		{
			if (this->_window != nullptr)
				glfwDestroyWindow(this->_window);

			glfwTerminate();
		}
	} // namespace Front
} // namespace Vox