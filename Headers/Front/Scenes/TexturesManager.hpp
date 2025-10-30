#ifndef __TEXTURESMANAGER_HPP__
#define __TEXTURESMANAGER_HPP__

#include <unordered_map>
#include <iostream>

#include "Front/Utils/ATexturesAtlas.hpp"
// #include "Front/Utils/MaskedTexturesAtlas.hpp"

#include "Front/Rendering/Images/FontImage.hpp"
#include "Game/Scenes/World/Skybox/SkyTexture.hpp"

#include <memory>

#include "Front/Utils/TexturesData.hpp"

#include <array>
#include <bitset>

namespace Vox::Front::Scenes
{
    class TexturesManager
    {
        public:
            TexturesManager(const std::string &T_Path);
            ~TexturesManager();
            
            void CreateMap(const std::string &T_Path);

            Utils::ATexturesAtlas *operator[](const std::string &key) {return this->_texturesMap[key];};
            const Utils::ATexturesAtlas &operator[](const std::string &key) const {return *this->_texturesMap.at(key);};

            Front::Rendering::Images::FontImage &GetFont() {return *this->_fontImage;};
            Game::World::Skybox::SkyTexture &GetSkyTexture() {return *this->_skyImage;};

            int AddDynamicImage(VkImageView &view);
            void RemoveDynamicImage(const size_t &index);

            inline const std::array<VkImageView, Utils::TexturesData::MAX_DYNAMIC_TEXTURES> &GetDynamics()
            {
                return this->_dynamicImages;
            }

            inline const VkSampler &GetDynamicSampler() {return this->_dynamicSampler;};

        protected:
            void CreateDynamicSampler();

            VkSampler _dynamicSampler = VK_NULL_HANDLE;

            std::array<VkImageView, Utils::TexturesData::MAX_DYNAMIC_TEXTURES> _dynamicImages;
            std::bitset<Utils::TexturesData::MAX_DYNAMIC_TEXTURES> _avalaibleDynamicImages;

            std::unique_ptr<Front::Rendering::Images::FontImage>_fontImage;
            std::unique_ptr<Game::World::Skybox::SkyTexture> _skyImage;
            std::unordered_map<std::string, Utils::ATexturesAtlas *> _texturesMap;
            /* private */
    
    };
    
    
    
} // namespace Vox::Front::Scenes


#endif // __TEXTURESMANAGER_HPP__