#ifndef __STRUCTURESMANAGER_HPP__
#define __STRUCTURESMANAGER_HPP__

#include <unordered_map>
#include <cstdint>

#include "./StructuresTypes.hpp"
#include "./Structure.hpp"

#include "MathGraphicalLib/Vectors/Vector3.hpp"

namespace Vox::Game::Datas::Structures
{
    class StructuresManager
    {
        public:
            StructuresManager();
            ~StructuresManager();
    
            const Structure &GetStructure(StructuresType type) const {return this->_list.at(type);};

        private:
            void LoadStructures();
            std::unordered_map<StructuresType, Structure> _list;
            /* private */
    
    };
} // namespace Vox::Game::Datas::Structures



#endif // __STRUCTURESMANAGER_HPP__