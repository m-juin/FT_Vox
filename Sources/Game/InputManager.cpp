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
		glfwSetWindowUserPointer(win, this);
		glfwSetCursorPosCallback(win,
								 [](GLFWwindow *window, double xPos, double yPos)
								 {
									 auto *inputMgr = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
									 if (inputMgr->_target == E_InputTarget::UI)
									 {
										 const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
										 im.HandleMouseMove(xPos, yPos);
									 }
									 else
									 {
									 }
								 });

		glfwSetMouseButtonCallback(win,
								   [](GLFWwindow *window, int button, int action, int mods)
								   {
									   (void)mods;
									   auto *inputMgr = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
									   if (inputMgr->_target == E_InputTarget::UI)
									   {
										   const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
										   im.HandleMouseClick(button, action);
									   }
								   });
		glfwSetScrollCallback(win,
							  [](GLFWwindow *window, double xoffset, double yoffset)
							  {
								  auto *inputMgr = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
								  if (inputMgr->_target == E_InputTarget::UI)
								  {
									  const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
									  im.HandleMouseScroll(xoffset, yoffset);
								  }
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
							   (void)mods;
							   (void)scancode;
							   auto *inputMgr = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
							   if (inputMgr->_target == E_InputTarget::UI)
							   {
								   const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
								   im.HandleKeyInput(key, action);
							   }
						   });
	}
} // namespace Vox::Game