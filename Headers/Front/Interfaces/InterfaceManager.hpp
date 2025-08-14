#ifndef __INTERFACEMANAGER_HPP__
#define __INTERFACEMANAGER_HPP__

#include "Utils/Singleton.hpp"

namespace Vox::Front::Interfaces
{
    class InterfaceManager : public Utils::Singleton<InterfaceManager>
    {
        friend class Utils::Singleton<InterfaceManager>;

        public:
            InterfaceManager();
            ~InterfaceManager();
    
        private:
            /* private */
    
    };
}

#endif // __INTERFACEMANAGER_HPP__