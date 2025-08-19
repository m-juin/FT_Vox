#ifndef __ATEXTURESMANAGER_HPP__
#define __ATEXTURESMANAGER_HPP__

#include <unordered_map>
#include <iostream>

#include "Front/Utils/TexturesAtlas.hpp"

namespace Vox::Front::Scenes
{
    class ATexturesManager
    {
        public:
            ATexturesManager() {};
            virtual ~ATexturesManager() {};
            
            virtual void CreateMap() = 0;

            Utils::TexturesAtlas *operator[](const std::string &key) {return this->_atlasMap[key];};

        protected:
            std::unordered_map<std::string, Utils::TexturesAtlas *> _atlasMap;
            /* private */
    
    };
} // namespace Vox::Front::Scenes


#endif // __ATEXTURESMANAGER_HPP__