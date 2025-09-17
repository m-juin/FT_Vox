#include "Front/Interfaces/Elements/DynamicImage.hpp"
namespace Vox::Front::Interfaces::Elements
{
    DynamicImage::DynamicImage(const Constructor &st) : AElement(st.pos, st.size)
    {
        
    }
    
    void DynamicImage::SetPos(const Vector2 newPos)
    {
        if (this->_pos == newPos)
            return ;
        this->_pos = newPos;
    }
    
    void DynamicImage::SetSize(const Vector2 newSize)
    {
        if (this->_size == newSize)
            return ;
        this->_size = newSize;
    }
    
    DynamicImage::~DynamicImage()
    {
        
    }
}