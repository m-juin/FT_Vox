#ifndef __INTERFACESMANAGER_HPP__
#define __INTERFACESMANAGER_HPP__

#include "Utils/Singleton.hpp"

#include <set>

#include "AInterface.hpp"

namespace Vox::Front::Interfaces
{
    class InterfacesManager : public Vox::Utils::Singleton<InterfacesManager>
    {
        friend class Vox::Utils::Singleton<InterfacesManager>;

        public:
            void ResetInterfacesList();
            void RegisterInterface(AInterface *inte);
            void Render();

            void HandleMouseMove(const size_t &xPos, const size_t &yPos) const;
            InterfacesManager();
            ~InterfacesManager();
    
        private:;
            std::set<AInterface *> _content;
            /* private */
    
    };
}

#endif // __INTERFACESMANAGER_HPP__