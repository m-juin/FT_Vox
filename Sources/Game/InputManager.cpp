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
		this->_target = E_InputTarget::Camera;
		LoadInput();
		SetupCallBack();
		onUpdate.AddCallBack([this]() { this->HandlePerFrameInput(); });
	}

	InputManager::~InputManager() {}

	bool InputManager::HandleNonTargetInput(const int &button, const int &action)
	{
		if (button != this->_inputMap[E_InputAction::I_F3] && button != this->_inputMap[E_InputAction::I_Generation])
			return false;
		if (action != GLFW_RELEASE)
			return true;
		E_InputAction inputAction = button == this->_inputMap[E_InputAction::I_F3] ? E_InputAction::I_F3
									: E_InputAction::I_Generation;
		Game::GameManager::GetInstance().GetSceneManager().GetCurrentScene().HandleInputAction(inputAction);
		return true;
	}

	InputManager &InputManager::GetInstance()
	{
		return Game::GameManager::GetInstance().GetInputManager();
	}

	void InputManager::SetupCallBack()
	{
		auto win = Front::Window::GetInstance().GetWindow();
		glfwSetWindowUserPointer(win, this);
		glfwSetCursorPosCallback(win,
								 [](GLFWwindow *window, double xPos, double yPos)
								 {
									 const auto *inputMgr =
										 static_cast<InputManager *>(glfwGetWindowUserPointer(window));
									 if ((inputMgr->_inputMask & Game::Utils::Datas::InputMask::Mouse) != 00000000)
									 {
										 const auto &im = Front::Interfaces::InterfacesManager::GetInstance();
										 float xScale, yScale;
										 glfwGetWindowContentScale(window, &xScale, &yScale);
										 im.HandleMouseMove(xPos * xScale, yPos * yScale);
									 }
									 else
									 {
										 auto [width, height] = Front::Rendering::SwapChain::GetInstance().GetExtent();
										 const double centerX = width / 2;
										 const double centerY = height / 2;
										 if (std::abs(xPos - centerX) < 0.001 && std::abs(yPos - centerY) < 0.001)
											 return;
										 const double dx = centerX - xPos;
										 const double dy = yPos - centerY;
										 auto &camera = World::WorldManager::GetCamera();
										 camera.HandleMouseMovement(dx, dy);
										 glfwSetCursorPos(window, centerX, centerY);
									 }
								 });

		glfwSetMouseButtonCallback(win,
								   [](GLFWwindow *window, int button, int action, int mods)
								   {
									   (void)mods;
									   const auto *inputMgr =
										   static_cast<InputManager *>(glfwGetWindowUserPointer(window));
									   if ((inputMgr->_inputMask & Game::Utils::Datas::InputMask::Mouse) != 00000000)
									   {
										   auto &im = Front::Interfaces::InterfacesManager::GetInstance();
										   im.HandleMouseClick(button, action);
									   }
								   });
		glfwSetScrollCallback(win,
							  [](GLFWwindow *window, double xoffset, double yoffset)
							  {
								  auto *inputMgr = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
								  if ((inputMgr->_inputMask & Game::Utils::Datas::InputMask::Mouse) != 00000000)
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
							   if (inputMgr->HandleNonTargetInput(key, action))
								   return;
							   if ((inputMgr->_inputMask & Game::Utils::Datas::InputMask::KeyBoard) != 00000000)
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
		this->_inputMap[E_InputAction::MOVE_DOWN] = GLFW_KEY_LEFT_ALT;

		this->_inputMap[E_InputAction::MOVE_LEFT] = GLFW_KEY_A;
		this->_inputMap[E_InputAction::MOVE_RIGHT] = GLFW_KEY_D;

		this->_inputMap[E_InputAction::I_F3] = GLFW_KEY_F3;
		this->_inputMap[E_InputAction::I_Generation] = GLFW_KEY_F4;
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

	void InputManager::UpdateInputMask(const uint8_t &newMask)
	{
		if (newMask == this->_inputMask)
			return;
		this->_inputMask = newMask;
		auto win = Front::Window::GetInstance().GetWindow();
		if ((newMask & Game::Utils::Datas::InputMask::Mouse) != 00000000)
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void InputManager::HandlePerFrameInput()
	{
		if (this->_inputMask & Game::Utils::Datas::InputMask::KeyBoard)
			return;

		auto win = Front::Window::GetInstance().GetWindow();
		auto &camera = Game::World::WorldManager::GetCamera();
		if (glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_UP]) == GLFW_PRESS)
			camera.Move({0, 1, 0});
		if (glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_DOWN]) == GLFW_PRESS)
			camera.Move({0, -1, 0});
		if (glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_RIGHT]) == GLFW_PRESS)
			camera.Move({1, 0, 0});
		if (glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_LEFT]) == GLFW_PRESS)
			camera.Move({-1, 0, 0});
		if (glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_FRONT]) == GLFW_PRESS)
			camera.Move({0, 0, -1});
		if (glfwGetKey(win, this->_inputMap[E_InputAction::MOVE_BACK]) == GLFW_PRESS)
			camera.Move({0, 0, 1});
	}
} // namespace Vox::Game
