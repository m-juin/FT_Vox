#ifndef __INTERFACESMANAGER_HPP__
#define __INTERFACESMANAGER_HPP__

#include "Utils/Singleton.hpp"

#include <vector>

#include "AInterface.hpp"

namespace Vox::Front::Interfaces
{
    class InterfacesManager : public Utils::Singleton<InterfacesManager>
    {
        friend class Utils::Singleton<InterfacesManager>;

        public:
            InterfacesManager();
            ~InterfacesManager();
    
        private:;
            std::vector<AInterface *> _content;
            /* private */
    
    };
}

#endif // __INTERFACESMANAGER_HPP__