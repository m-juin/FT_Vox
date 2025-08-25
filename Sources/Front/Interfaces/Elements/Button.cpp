#include "Front/Interfaces/Elements/Button.hpp"

#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include <GLFW/glfw3.h>

namespace Vox::Front::Interfaces::Elements
{

	Button::Button(Vector2 pos_, Vector2 size_, const std::string &content_, Color textColor_, Color bgColor_,
				   Color hoverBGColor_, Color hoverTXTColor_)
		: AContainer(pos_, size_)
	{
		this->AddElement("IMG_BackGround", std::make_unique<Image>("", "", this->_pos, this->_size, bgColor_), 0);

		Text::Vox_Text_Constructor stText{};
		stText.color = textColor_;
		stText.content = content_;
		stText.pos = this->_pos;
		stText.size = this->_size;

		this->AddElement("TXT_Content", std::make_unique<Text>(stText), 1);

		this->_onHoverCallbacks.AddCallBack([this, hoverBGColor_, hoverTXTColor_](void)
        {
			std::cout << "newColor:\n\tBG =" << hoverBGColor_ << "\n\tFG =" << hoverTXTColor_ << std::endl;
            this->GetElement<Image>("IMG_BackGround")->SetColor(hoverBGColor_);
            this->GetElement<Text>("TXT_Content")->SetColor(hoverTXTColor_);
        });

		this->_onHoverLeaveCallbacks.AddCallBack([this, bgColor_, textColor_](void)
        {
			std::cout << "newColor:\n\tBG =" << bgColor_ << "\n\tFG =" << textColor_ << std::endl;
            this->GetElement<Image>("IMG_BackGround")->SetColor(bgColor_);
            this->GetElement<Text>("TXT_Content")->SetColor(textColor_);
        });
	}

	Button::Button(const Vox_Button_Constructor &st) : AContainer(st.pos, st.size)
	{
		this->AddElement("IMG_BackGround", std::make_unique<Image>("", "", this->_pos, this->_size, st.bgColor), 0);

		Text::Vox_Text_Constructor stText{};
		stText.color = st.textColor;
		stText.content = st.content;
		Vector2 textSize = Text::GetTextSize(st.content, 0.5);
		stText.pos = {this->_pos[0] + (this->_size[0] / 2) - textSize[0] / 2, this->_pos[1] + (this->_size[1] / 2)};
		stText.size = this->_size;

		this->AddElement("TXT_Content", std::make_unique<Text>(stText), 1);

		this->_onHoverCallbacks.AddCallBack([this, st](void)
        {
			std::cout << "newColor:\n\tBG =" << st.hoverBGColor << "\n\tFG =" << st.hoverTXTColor << std::endl;
            this->GetElement<Image>("IMG_BackGround")->SetColor(st.hoverBGColor);
			this->GetElement<Text>("TXT_Content")->SetColor(st.hoverTXTColor);
        });

		this->_onHoverLeaveCallbacks.AddCallBack([this, st](void)
        {
			std::cout << "newColor:\n\tBG =" << st.bgColor << "\n\tFG =" << st.textColor << std::endl;
            this->GetElement<Image>("IMG_BackGround")->SetColor(st.bgColor);
			this->GetElement<Text>("TXT_Content")->SetColor(st.textColor);
        });
	}

	Button::~Button() {}

	void Button::OnClick(const int &button, const int &action)
	{
		if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
			return;

		std::cout << "[DEBUG] " << "Button is clicked." << std::endl;
	}

	void Button::OnHover()
	{
        this->_onHoverCallbacks.Notify();
		std::cout << "[DEBUG] " << "Button is Hovered." << std::endl;
	}

	void Button::OnHoverLeave()
	{
        this->_onHoverLeaveCallbacks.Notify();
		std::cout << "[DEBUG] " << "Button stopped Hovered." << std::endl;
	}

	bool Button::IsHover(const Vector2 &mousePos)
	{
		return AClickable::IsHover(mousePos);
	}

	void Button::SetPos(Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->_content[0].elem.get()->SetPos(this->_pos);
		this->_content[1].elem.get()->SetPos(this->_pos);
	}

	void Button::SetSize(Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->_content[0].elem.get()->SetSize(this->_size);
		this->_content[1].elem.get()->SetSize(this->_size);
	}

	void Button::ResetVertex() {}
} // namespace Vox::Front::Interfaces::Elements