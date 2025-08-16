#ifndef __INTERFACESMANAGER_HPP__
#define __INTERFACESMANAGER_HPP__

#include "Utils/Singleton.hpp"

#include <set>

#include "AInterface.hpp"

namespace Vox::Front::Interfaces
{
    class InterfacesManager : public Utils::Singleton<InterfacesManager>
    {
        friend class Utils::Singleton<InterfacesManager>;

        public:
            void ResetInterfacesList();
            void RegisterInterface(AInterface *inte);
            InterfacesManager();
            ~InterfacesManager();
    
        private:;
            std::set<AInterface *> _content;
            /* private */
    
    };
}

#endif // __INTERFACESMANAGER_HPP__