#ifndef __DUMMYIMAGE_HPP__
#define __DUMMYIMAGE_HPP__

#include "./VulkanImage.hpp"
#include "Utils/Singleton.hpp"

namespace Vox::Front::Rendering::Images
{
    class DummyImage : public virtual Vox::Utils::Singleton<DummyImage>, public virtual VulkanImage
    {
        friend class Vox::Utils::Singleton<DummyImage>;
        public:
            DummyImage();
            ~DummyImage();
    
        private:
            /* private */
    
    };
}

#endif // __DUMMYIMAGE_HPP__