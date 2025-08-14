#include "Front/Interfaces/Elements/Image.hpp"

#include "Front/Rendering/SwapChain.hpp"

#include "Front/Interfaces/Utils/Maths.hpp"

namespace Vox::Front::Interfaces::Elements
{
    Image::Image(Vector2 pos = {0, 0}, Vector2 size = {100, 50}) : Bases::AElement(pos, size)
    {
        const Vector2 screenSize(Rendering::SwapChain::GetInstance().GetExtent().width, Rendering::SwapChain::GetInstance().GetExtent().height);

        this->vertex[0] = vert(Utils::Maths::PointPixelToVulkan(this->_pos, screenSize), {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        this->vertex[1] = vert(Utils::Maths::PointPixelToVulkan(this->_pos, screenSize), {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        this->vertex[2] = vert(Utils::Maths::PointPixelToVulkan(this->_pos, screenSize), {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
        
    }

    Image::~Image()
    {
    }
} // namespace Vox::Front::Interfaces::Elements