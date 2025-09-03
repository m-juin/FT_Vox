#include "Game/InputManager.hpp"

#include <GLFW/glfw3.h>

#include "Front/Window.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"
#include "Game/Scenes/World/WorldManager.hpp"

#include "Front/Rendering/SwapChain.hpp"

#include <cmath>

namespace Vox::Game
{
	InputManager::InputManager() : Vox::Utils::AUpdatable(1)
	{
		LoadInput();
		SetupCallBack();
		onUpdate.AddCallBack([this]()
	{
		this->HandlePerFrameInput();
	});
	}

	InputManager::~InputManager() {}

	void InputManager::SetupCallBack()
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
										 auto extent = Front::Rendering::SwapChain::GetInstance().GetExtent();
										 double centerX = extent.width / 2;
										 double centerY = extent.height / 2;
										 if (std::abs(xPos - centerX) < 0.001 && std::abs(yPos - centerY) < 0.001)
											 return;
										 double dx = centerX - xPos;
										 double dy = yPos - centerY;
										 auto &camera = Game::World::WorldManager::GetCamera();
										 camera.HandleMouseMovement(dx, dy);
										 glfwSetCursorPos(window, centerX, centerY);
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

	void InputManager::LoadInput()
	{
		this->_inputMap[E_InputAction::MOVE_FRONT] = GLFW_KEY_W;
		this->_inputMap[E_InputAction::MOVE_BACK] = GLFW_KEY_S;

		this->_inputMap[E_InputAction::MOVE_UP] = GLFW_KEY_SPACE;
		this->_inputMap[E_InputAction::MOVE_DOWN] = GLFW_KEY_LEFT_SHIFT;

		this->_inputMap[E_InputAction::MOVE_LEFT] = GLFW_KEY_A;
		this->_inputMap[E_InputAction::MOVE_RIGHT] = GLFW_KEY_D;
	}

	void InputManager::SetInputTarget(E_InputTarget newTarget)
	{
		if (newTarget == this->_target)
			return;
		this->_target = newTarget;
		auto win = Front::Window::GetInstance().GetWindow();
		if (this->_target == E_InputTarget::UI)
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else if (this->_target == E_InputTarget::Camera)
		{
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			auto extent = Front::Rendering::SwapChain::GetInstance().GetExtent();
			glfwSetCursorPos(win, extent.width / 2, extent.height / 2);
		}
	}
	
	void InputManager::HandlePerFrameInput()
	{
		if (this->_target == E_InputTarget::UI)
			return ;

		auto win = Front::Window::GetInstance().GetWindow();
		auto &camera = Game::World::WorldManager::GetCamera();
		if(glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_UP]) == GLFW_PRESS)
			camera.Move({0, 1, 0});
		if(glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_DOWN]) == GLFW_PRESS)
			camera.Move({0, -1, 0});
		if(glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_RIGHT]) == GLFW_PRESS)
			camera.Move({1, 0, 0});
		if(glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_LEFT]) == GLFW_PRESS)
			camera.Move({-1, 0, 0});
		if(glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_FRONT]) == GLFW_PRESS)
			camera.Move({0, 0, -1});
		if(glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_BACK]) == GLFW_PRESS)
			camera.Move({0, 0, 1});

	}


} // namespace Vox::Game