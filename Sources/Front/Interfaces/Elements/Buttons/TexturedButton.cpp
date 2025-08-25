#include "Front/Interfaces/Elements/Buttons/TexturedButton.hpp"

#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

namespace Vox::Front::Interfaces::Elements::Buttons
{
	TexturedButton::TexturedButton(const Vox_TexturedButton_Constructor &st) : AButton(st)
	{
		this->AddElement("IMG_BackGround", std::make_unique<Image>(st.atlas, st.atlasKey, this->_pos, this->_size), 0);

		this->_onHoverCallbacks.AddCallBack(
			[this, st](void)
			{
				// std::cout << "newColor:\n\tBG =" << st.hoverBGColor << "\n\tFG =" << st.hoverTXTColor << std::endl;
				this->GetElement<Image>("IMG_BackGround")->SetTexture(st.onHoverAtlas, st.onHoverAtlasKey);
				this->GetElement<Text>("TXT_Content")->SetColor(st.hoverTXTColor);
			});

		this->_onHoverLeaveCallbacks.AddCallBack(
			[this, st](void)
			{
				// std::cout << "newColor:\n\tBG =" << st.bgColor << "\n\tFG =" << st.textColor << std::endl;
				this->GetElement<Image>("IMG_BackGround")->SetTexture(st.atlas, st.atlasKey);
				this->GetElement<Text>("TXT_Content")->SetColor(st.textColor);
			});
	}

	TexturedButton::~TexturedButton() {}
} // namespace Vox::Front::Interfaces::Elements::Buttons