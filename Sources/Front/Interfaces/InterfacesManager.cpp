#include "Front/Interfaces/InterfacesManager.hpp"

#include "Front/Interfaces/Menu.hpp"

#include "Front/Rendering/SwapChain.hpp"

namespace Vox::Front::Interfaces
{
    InterfacesManager::InterfacesManager()
    {
        const Vector2 screenSize(Rendering::SwapChain::GetInstance().GetExtent().width,
								 Rendering::SwapChain::GetInstance().GetExtent().height);

        this->_content.push_back(new Menu({0, 0}, screenSize));
    }
    
    InterfacesManager::~InterfacesManager()
    {
        for (AInterface *interface : this->_content)
            delete interface;
    }
}