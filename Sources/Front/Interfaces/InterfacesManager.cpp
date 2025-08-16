#include "Front/Interfaces/InterfacesManager.hpp"

#include "Front/Rendering/SwapChain.hpp"

namespace Vox::Front::Interfaces
{
	void InterfacesManager::ResetInterfacesList()
	{
		for (AInterface *interface : this->_content)
			delete interface;
	}

	void InterfacesManager::RegisterInterface(AInterface *inte)
    {
        this->_content.insert(inte);
    }

	InterfacesManager::InterfacesManager() {}

	InterfacesManager::~InterfacesManager()
	{
		this->ResetInterfacesList();
	}
} // namespace Vox::Front::Interfaces