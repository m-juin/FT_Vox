#include "Front/Interfaces/InterfacesManager.hpp"

#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"

#include <GLFW/glfw3.h>

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
        for (auto& pair : this->_content) // Utilisez auto& pour éviter les copies
        {
            if (pair.second->IsEnabled())
            {
                pair.second->Render();
            }
        }
    }
	
	void InterfacesManager::HandleMouseMove(const size_t &xPos, const size_t &yPos) const
	{
		for (auto pair : this->_content)
			pair.second->IsHover({xPos, yPos});
	}
	
	void InterfacesManager::HandleMouseClick(const int &button, const int &action) const
	{
		for (auto pair : this->_content)
			if (pair.second->IsEnabled() == true)
				pair.second->OnClick(button, action);
	}

	InterfacesManager::InterfacesManager() {}

	InterfacesManager::~InterfacesManager()
	{
		// this->ResetInterfacesList();
	}
} // namespace Vox::Front::Interfaces