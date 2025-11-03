#include "Front/Interfaces/Elements/Buttons/AButton.hpp"

#include "Front/Interfaces/Elements/Text.hpp"

#include <GLFW/glfw3.h>

namespace Vox::Front::Interfaces::Elements::Buttons
{

	AButton::AButton(const std::string &content_, Color textColor_)
	{

		Text::Vox_Text_Constructor stText{};
		stText.color = textColor_;
		stText.content = content_;
		stText.pos = this->_pos;
		stText.size = this->_size;

		this->AddElement("TXT_Content", std::make_unique<Text>(stText), 1);
	}

	AButton::AButton(const Vox_Button_Constructor &st)
	{

		Text::Vox_Text_Constructor stText{};
		stText.color = st.textColor;
		stText.content = st.content;
		Vector2 textSize = Text::GetTextSize(st.content, st.textScale);
		stText.pos = {this->_pos[0] + (this->_size[0] / 2) - textSize[0] / 2,
					  this->_pos[1] + (size_t)((this->_size[1] / 2) + (8 * st.textScale))};
		stText.size = this->_size;
		stText.scale = st.textScale;

		this->AddElement("TXT_Content", std::make_unique<Text>(stText), 1);
	}

	AButton::~AButton() {}


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
} // namespace Vox::Front::Interfaces::Elements::Buttons