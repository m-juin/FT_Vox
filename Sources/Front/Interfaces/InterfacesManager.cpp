#include "Front/Interfaces/InterfacesManager.hpp"

#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"

namespace Vox::Front::Interfaces
{
	void InterfacesManager::ResetInterfacesList()
	{
		for (AInterface *interface : this->_content)
			if (interface)
				delete interface;
	}

	void InterfacesManager::RegisterInterface(AInterface *inte)
    {
        this->_content.insert(inte);
    }
	
	void InterfacesManager::Render()
	{
		Rendering::Pipelines::PipelinesManager::GetInstance().BindPipeline("StaticGUI");
		for (auto interface : this->_content)
			interface->Render();
	}

	InterfacesManager::InterfacesManager() {}

	InterfacesManager::~InterfacesManager()
	{
		// this->ResetInterfacesList();
	}
} // namespace Vox::Front::Interfaces