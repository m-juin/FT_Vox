#ifndef __ATEXTURESMANAGER_HPP__
#define __ATEXTURESMANAGER_HPP__

#include <unordered_map>
#include <iostream>

namespace Vox::Front::Scenes
{
    class ATexturesManager
    {
        public:
            ATexturesManager() {};
            ~ATexturesManager() {};
            
            virtual void CreateMap() = 0;

        private:
            std::unordered_map<std::string, TexturesAtlas> _atlasMap
            /* private */
    
    };
} // namespace Vox::Front::Scenes


#endif // __ATEXTURESMANAGER_HPP__