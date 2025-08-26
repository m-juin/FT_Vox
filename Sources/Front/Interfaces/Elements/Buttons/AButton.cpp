#include "Front/Interfaces/Elements/Buttons/AButton.hpp"

#include "Front/Interfaces/Elements/Text.hpp"

#include <GLFW/glfw3.h>

namespace Vox::Front::Interfaces::Elements::Buttons
{

	AButton::AButton(Vector2 pos_, Vector2 size_, const std::string &content_, Color textColor_)
		: AContainer(pos_, size_)
	{
		// this->AddElement("IMG_BackGround", std::make_unique<Image>("", "", this->_pos, this->_size, bgColor_), 0);

		Text::Vox_Text_Constructor stText{};
		stText.color = textColor_;
		stText.content = content_;
		stText.pos = this->_pos;
		stText.size = this->_size;

		this->AddElement("TXT_Content", std::make_unique<Text>(stText), 1);

		// this->_onHoverCallbacks.AddCallBack([this, hoverBGColor_, hoverTXTColor_](void)
        // {
		// 	std::cout << "newColor:\n\tBG =" << hoverBGColor_ << "\n\tFG =" << hoverTXTColor_ << std::endl;
        //     this->GetElement<Image>("IMG_BackGround")->SetColor(hoverBGColor_);
        //     this->GetElement<Text>("TXT_Content")->SetColor(hoverTXTColor_);
        // });

		// this->_onHoverLeaveCallbacks.AddCallBack([this, bgColor_, textColor_](void)
        // {
		// 	std::cout << "newColor:\n\tBG =" << bgColor_ << "\n\tFG =" << textColor_ << std::endl;
        //     this->GetElement<Image>("IMG_BackGround")->SetColor(bgColor_);
        //     this->GetElement<Text>("TXT_Content")->SetColor(textColor_);
        // });
	}

	AButton::AButton(const Vox_Button_Constructor &st) : AContainer(st.pos, st.size)
	{
		// this->AddElement("IMG_BackGround", std::make_unique<Image>("", "", this->_pos, this->_size, st.bgColor), 0);

		Text::Vox_Text_Constructor stText{};
		stText.color = st.textColor;
		stText.content = st.content;
		Vector2 textSize = Text::GetTextSize(st.content, st.textScale);
		stText.pos = {this->_pos[0] + (this->_size[0] / 2) - textSize[0] / 2, this->_pos[1] + (size_t)((this->_size[1] / 2) + (8 * st.textScale))};
		stText.size = this->_size;
		stText.scale = st.textScale;

		this->AddElement("TXT_Content", std::make_unique<Text>(stText), 1);
	}

	AButton::~AButton() {}

	void AButton::OnClick(const int &button, const int &action)
	{
		this->onClickCallbacks.Notify(button, action);

		// std::cout << "[DEBUG] " << "Button is clicked." << std::endl;
	}

	void AButton::OnHover()
	{
        this->_onHoverCallbacks.Notify();
		std::cout << "[DEBUG] " << "Button is Hovered." << std::endl;
	}

	void AButton::OnHoverLeave()
	{
        this->_onHoverLeaveCallbacks.Notify();
		std::cout << "[DEBUG] " << "Button stopped Hovered." << std::endl;
	}

	bool AButton::IsHover(const Vector2 &mousePos)
	{
		return AClickable::IsHover(mousePos);
	}

	void AButton::SetPos(Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->_content[0].elem.get()->SetPos(this->_pos);
		this->_content[1].elem.get()->SetPos(this->_pos);
	}

	void AButton::SetSize(Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->_content[0].elem.get()->SetSize(this->_size);
		this->_content[1].elem.get()->SetSize(this->_size);
	}

	void AButton::ResetVertex() {}
} // namespace Vox::Front::Interfaces::Elements