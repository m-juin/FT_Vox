#include "Front/Interfaces/InterfacesManager.hpp"

#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/SwapChain.hpp"

#include "Front/Interfaces/Elements/Bases/AClickable.hpp"
#include "Front/Interfaces/Elements/Bases/AFocusable.hpp"
#include "Front/Interfaces/Elements/InputField.hpp"

#include <GLFW/glfw3.h>

namespace Vox::Front::Interfaces::Elements::Bases
{
	bool AClickable::_hoverConsummed = false;
	bool AClickable::_clickConsummed = false;
	bool AFocusable::_focusConsummed = false;
	AFocusable *AFocusable::_focusedElement = nullptr;
} // namespace Vox::Front::Interfaces::Elements::Bases

namespace Vox::Front::Interfaces
{
	void InterfacesManager::ResetInterfacesList()
	{
		for (auto pair : this->_content)
			if (pair.second)
				delete pair.second;
	}

	void InterfacesManager::RegisterInterface(const std::string &key, AInterface *inte)
	{
		this->_content.insert({key, inte});
	}

	void InterfacesManager::Render()
	{
		Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("StaticGUI");
		for (auto &pair : this->_content) // Utilisez auto& pour éviter les copies
		{
			if (pair.second->IsEnabled())
			{
				pair.second->Render();
			}
		}
	}

	void InterfacesManager::HandleMouseMove(const float &xPos, const float &yPos) const
	{
		Front::Interfaces::Elements::Bases::AClickable::ResetHoverState();
		for (auto pair : this->_content)
		{
			pair.second->IsHover({xPos, yPos});
		}
	}

	void InterfacesManager::HandleMouseScroll(const double &xOff, const double &yOff) const
	{
		// std::cout << "here\n" << std::endl;
		for (auto pair : this->_content)
			if (pair.second->IsEnabled() == true)
				pair.second->OnScroll(xOff, yOff);
	}

	void InterfacesManager::HandleMouseClick(const int &button, const int &action) const
	{
		Front::Interfaces::Elements::Bases::AClickable::ResetClickState();
		Front::Interfaces::Elements::Bases::AFocusable::ResetFocusConsumtion();
		for (auto pair : this->_content)
			if (pair.second->IsEnabled() == true)
				pair.second->OnClick(button, action);
		if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
			return ;
		if (Front::Interfaces::Elements::Bases::AFocusable::_focusConsummed == false)
			Front::Interfaces::Elements::Bases::AFocusable::SetFocusElement(nullptr);
	}

	void InterfacesManager::HandleCharInput(const unsigned int &codePoint) const
	{
		auto focused = Front::Interfaces::Elements::Bases::AFocusable::GetFocused();
		if (auto elem = dynamic_cast<Elements::InputField *>(focused))
			elem->HandleCharInput(codePoint);
	}

	void InterfacesManager::HandleKeyInput(const unsigned int &codePoint, const int &action) const
	{
		auto focused = Front::Interfaces::Elements::Bases::AFocusable::GetFocused();
		if (auto elem = dynamic_cast<Elements::InputField *>(focused))
			elem->HandleKeyInput(codePoint, action);
	}

	InterfacesManager::InterfacesManager() {}

	InterfacesManager::~InterfacesManager()
	{
		// this->ResetInterfacesList();
	}
} // namespace Vox::Front::Interfaces