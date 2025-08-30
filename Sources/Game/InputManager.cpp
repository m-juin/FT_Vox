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
		auto win = Front::Window::GetInstance().GetWindow();
		glfwSetCursorPosCallback(win,
								 [](GLFWwindow *window, double xPos, double yPos)
								 {
									 (void)window;
									 const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
									 im.HandleMouseMove(xPos, yPos);
								 });

		glfwSetMouseButtonCallback(win,
								   [](GLFWwindow *window, int button, int action, int mods)
								   {
									   (void)window;
									   (void)mods;
									   const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
									   im.HandleMouseClick(button, action);
								   });
		glfwSetScrollCallback(win,
							  [](GLFWwindow *window, double xoffset, double yoffset)
							  {
								  (void)window;
								  const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
								  im.HandleMouseScroll(xoffset, yoffset);
							  });

		glfwSetCharCallback(win,
							[](GLFWwindow *window, unsigned int codepoint)
							{
								(void)window;
								const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
								im.HandleCharInput(codepoint);
							});
		glfwSetKeyCallback(win,
							[](GLFWwindow *window, int key, int scancode, int action, int mods)
							{
								(void)window;
								(void)mods;
								(void)scancode;
								const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
								im.HandleKeyInput(key, action);
							});
	}
} // namespace Vox::Game