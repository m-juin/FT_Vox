#include "Game/InputManager.hpp"

#include <GLFW/glfw3.h>

#include "Front/Window.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"

namespace Vox::Game
{
	InputManager::InputManager()
	{
		LoadInput();
	}

	InputManager::~InputManager() {}

	void InputManager::LoadInput()
	{
		glfwSetCursorPosCallback(Front::Window::GetInstance().GetWindow(), [](GLFWwindow *window, double xPos, double yPos)
    {
        (void)window;
        const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
        im.HandleMouseMove(xPos, yPos);

    });
	}
} // namespace Vox::Game