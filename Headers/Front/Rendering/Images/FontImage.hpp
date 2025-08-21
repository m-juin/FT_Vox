#ifndef __FONTIMAGE_HPP__
#define __FONTIMAGE_HPP__

#include "./VulkanImage.hpp"

#include <iostream>

namespace Vox::Front::Rendering::Images
{
    class FontImage : public VulkanImage
    {
        public:
            FontImage() = delete;
            FontImage(const std::string &fontName);
            ~FontImage() {};
    
        private:
            /* private */
    
    };
} // namespace Vox::Front::Rendering::Images


#endif // __FONTIMAGE_HPP__