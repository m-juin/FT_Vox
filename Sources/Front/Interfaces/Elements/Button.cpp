#include "Front/Interfaces/Elements/Button.hpp"

#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include <GLFW/glfw3.h>

namespace Vox::Front::Interfaces::Elements
{

    Button::Button(Vector2 pos_, Vector2 size_, const std::string &content_, Color textColor_, Color bgColor_) : AContainer(pos_, size_)
    {
        this->AddElement("IMG_BackGround",
        std::make_unique<Image>("", "", this->_pos, this->_size, bgColor_), 0);
        
        Text::Vox_Text_Constructor stText{};
        stText.color = textColor_;
        stText.content = content_;
        stText.pos = this->_pos;
        stText.size = this->_size;

        this->AddElement("TXT_Content", 
        std::make_unique<Text>(stText), 1);
    }
    
    Button::Button(const Vox_Button_Constructor &st) : AContainer(st.pos, st.size)
    {
        this->AddElement("IMG_BackGround",
        std::make_unique<Image>("", "", this->_pos, this->_size, st.bgColor), 0);

        Text::Vox_Text_Constructor stText{};
        stText.color = st.textColor;
        stText.content = st.content;
        Vector2 textSize = Text::GetTextSize(st.content, 0.5);
        stText.pos = {this->_pos[0] + (this->_size[0] / 2) - textSize[0] / 2, this->_pos[1] + (this->_size[1] / 2)};
        stText.size = this->_size;

        this->AddElement("TXT_Content", 
        std::make_unique<Text>(stText), 1);
    }

    
    Button::~Button()
    {
        
    }
    
    void Button::OnClick(const int &button, const int &action)
    {
        if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE) return ;

        std::cout << "[DEBUG] " << "Button is clicked." << std::endl;
    }
    
    void Button::OnHover()
    {
        std::cout << "[DEBUG] " << "Button is Hovered." << std::endl;
    }

    void Button::OnHoverLeave()
    {
        std::cout << "[DEBUG] " << "Button stopped Hovered." << std::endl;
    }

    bool Button::IsHover(const Vector2 &mousePos)
    {
        return AClickable::IsHover(mousePos);
    }
    
    void Button::SetPos(Vector2 newPos)
    {
        if (newPos == this->_pos) return ;
        this->_pos = newPos;
        this->_content[0].elem.get()->SetPos(this->_pos);
        this->_content[1].elem.get()->SetPos(this->_pos);
    }
    
    void Button::SetSize(Vector2 newSize)
    {
        if (newSize == this->_size) return ;
        this->_size = newSize;
        this->_content[0].elem.get()->SetSize(this->_size);
        this->_content[1].elem.get()->SetSize(this->_size);
    }
    
    void Button::ResetVertex()
    {
        
    }
}