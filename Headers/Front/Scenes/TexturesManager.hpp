#ifndef __TEXTURESMANAGER_HPP__
#define __TEXTURESMANAGER_HPP__

#include <unordered_map>
#include <iostream>

#include "Front/Utils/TexturesAtlas.hpp"

#include "Front/Rendering/Images/FontImage.hpp"

#include <memory>

namespace Vox::Front::Scenes
{
    class TexturesManager
    {
        public:
            TexturesManager(const std::string &T_Path);
            ~TexturesManager();
            
            void CreateMap(const std::string &T_Path);

            Utils::TexturesAtlas *operator[](const std::string &key) {return this->_texturesMap[key];};

            Front::Rendering::Images::FontImage &GetFont() {return *this->_fontImage;};
        protected:
            std::unique_ptr<Front::Rendering::Images::FontImage>_fontImage;
            std::unordered_map<std::string, Utils::TexturesAtlas *> _texturesMap;
            /* private */
    
    };
    
} // namespace Vox::Front::Scenes


#endif // __TEXTURESMANAGER_HPP__