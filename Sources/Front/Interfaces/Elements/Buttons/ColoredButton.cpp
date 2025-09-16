#include "Front/Interfaces/Elements/Buttons/ColoredButton.hpp"

#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

namespace Vox::Front::Interfaces::Elements::Buttons
{

	ColoredButton::ColoredButton(const Vox_ColorButton_Constructor &st) : AElement(st.pos, st.size),  AButton(st)
	{
		this->AddElement("IMG_BackGround", std::make_unique<Image>("", "", this->_pos, this->_size, st.bgColor), 0);

		this->_onHoverCallbacks.AddCallBack(
			[this, st](void)
			{
				this->GetElement<Image>("IMG_BackGround")->SetColor(st.hoverBGColor);
				this->GetElement<Text>("TXT_Content")->SetColor(st.hoverTXTColor);
			});

		this->_onHoverLeaveCallbacks.AddCallBack(
			[this, st](void)
			{
				this->GetElement<Image>("IMG_BackGround")->SetColor(st.bgColor);
				this->GetElement<Text>("TXT_Content")->SetColor(st.textColor);
			});

		this->_onEnableStatusChangeCallbacks.AddCallBack(
			[this, st](const bool &newState)
			{
				if (newState == true)
				{
					this->GetElement<Image>("IMG_BackGround")->SetColor(st.bgColor);
					this->GetElement<Text>("TXT_Content")->SetColor(st.textColor);
				}
				else
				{
					this->GetElement<Image>("IMG_BackGround")->SetColor(st.disabledBGColor);
					this->GetElement<Text>("TXT_Content")->SetColor(st.disabledTXTColor);
				}
			});
	}
} // namespace Vox::Front::Interfaces::Elements::Buttons