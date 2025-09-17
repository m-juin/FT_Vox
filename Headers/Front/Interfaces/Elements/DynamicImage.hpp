#ifndef __DYNAMICIMAGE_HPP__
#define __DYNAMICIMAGE_HPP__

#include "./Bases/AElement.hpp"

namespace Vox::Front::Interfaces::Elements
{
    class DynamicImage : public virtual Bases::AElement
    {
        public:
            struct Constructor {
                Vector2 pos;
                Vector2 size;

                Constructor() : pos({0, 0}), size({50, 50}) {}; 
            };

            DynamicImage() = delete;
            DynamicImage(const Constructor &st);
            void SetPos(const Vector2 newPos) override;
            void SetSize(const Vector2 newSize) override;
            ~DynamicImage();
    
        private:
            /* private */
    
    };
} // namespace Vox::Front::Interfaces::Elements


#endif // __DYNAMICIMAGE_HPP__