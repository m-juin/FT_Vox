#include "Front/Interfaces/Elements/Buttons/ColoredButton.hpp"

#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

namespace Vox::Front::Interfaces::Elements::Buttons
{
	ColoredButton::ColoredButton(const Vox_ColorButton_Constructor &st) : AButton(st)
	{
		this->AddElement("IMG_BackGround", std::make_unique<Image>("", "", this->_pos, this->_size, st.bgColor), 0);

		this->_onHoverCallbacks.AddCallBack([this, st](void)
		{
			// std::cout << "newColor:\n\tBG =" << st.hoverBGColor << "\n\tFG =" << st.hoverTXTColor << std::endl;
		    this->GetElement<Image>("IMG_BackGround")->SetColor(st.hoverBGColor);
			this->GetElement<Text>("TXT_Content")->SetColor(st.hoverTXTColor);
		});

		this->_onHoverLeaveCallbacks.AddCallBack([this, st](void)
		{
			// std::cout << "newColor:\n\tBG =" << st.bgColor << "\n\tFG =" << st.textColor << std::endl;
		    this->GetElement<Image>("IMG_BackGround")->SetColor(st.bgColor);
			this->GetElement<Text>("TXT_Content")->SetColor(st.textColor);
		});
	}
} // namespace Vox::Front::Interfaces::Elements::Buttons