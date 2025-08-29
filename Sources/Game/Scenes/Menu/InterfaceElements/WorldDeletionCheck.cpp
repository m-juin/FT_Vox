#include "Game/Scenes/Menu/InterfacesElements/WorldDeletionCheck.hpp"

#include "Front/Interfaces/Elements/Buttons/ColoredButton.hpp"
#include "Front/Interfaces/Elements/Buttons/TexturedButton.hpp"
#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include "Front/Rendering/SwapChain.hpp"

#include <GLFW/glfw3.h>

namespace Vox::Game::Scenes::Menu::Interfaces::Elements
{
	WorldDeletionCheck::WorldDeletionCheck(const Vox_WorldDeletionCheck_Constructor &st)
		: Bases::AElement(st.pos, st.size), AClickable(), Bases::AContainer(), _linkedWorld(st.sd)
	{
		Buttons::ColoredButton::Vox_ColorButton_Constructor BTNpm{};
		auto extent = Front::Rendering::SwapChain::GetInstance().GetExtent();
		BTNpm.size = {(float)extent.width, (float)extent.height};
		BTNpm.bgColor = {0., 0., 0., 0.2f};
		BTNpm.disabledBGColor = {0., 0., 0., 0.2f};
		BTNpm.hoverBGColor = {0., 0., 0., 0.2f};

		auto button = std::make_unique<Buttons::ColoredButton>(BTNpm);
		// button->ChangeEnableState(false);
		this->AddElement("IMG_BG_Block", std::move(button), 0);

		this->AddElement("IMG_BG",
						 std::make_unique<Image>("Menu_Main", "Dirt", this->_pos, this->_size,
												 Color(0.6f, 0.6f, 0.6f, 1.0f), Vector2(4, 4)),
						 1);

		// std::cout << st.sd << std::endl;

		{ // TXT_Confirm + TXT_WN
			Text::Vox_Text_Constructor ElemSt{};
			ElemSt.content = "Are you sure you want to delete the world:";

			ElemSt.color = {0.5, 0.5, 0.5, 1.0};
			ElemSt.scale = 0.6f;
			ElemSt.pos = {this->_pos[0] + this->_size[0] / 2 - Text::GetTextSize(ElemSt.content, ElemSt.scale)[0] / 2,
						  this->_pos[1] + 35};
			ElemSt.size = (Text::GetTextSize(ElemSt.content, 0.7f)[0], 50);
			this->AddElement("TXT_Confirm", std::make_unique<Text>(ElemSt), 2);

			ElemSt.content = _linkedWorld.worldName;
			ElemSt.color = {1., 1., 1., 1.};
			ElemSt.pos = {this->_pos[0] + this->_size[0] / 2 - Text::GetTextSize(ElemSt.content, ElemSt.scale)[0] / 2,
						  this->_pos[1] + 90};
			ElemSt.size = (Text::GetTextSize(ElemSt.content, 0.7f)[0], 50);
			this->AddElement("TXT_WN", std::make_unique<Text>(ElemSt), 2);
		}
		{ // BTN_Confirm + BTN_Cancel
			Buttons::ColoredButton::Vox_ColorButton_Constructor pm{};

			// pm.atlas = "Menu_Main";
			// pm.atlasKey = "Button";
			pm.hoverTXTColor = {1.0, 1.0, 1.0, 1.0};
			pm.disabledBGColor = {1.0, 0.0, 0.0, 1.0};
			pm.bgColor = {0.0, 1.0, 0.0, 1.0};
			pm.hoverBGColor = {0.0, 0.0, 1.0, 1.0};
			// pm.onHoverAtlas = "Menu_Main";
			// pm.onHoverAtlasKey = "Button_Hover";
			pm.disabledTXTColor = {0.5, 0.5, 0.5, 1.0};
			// pm.onDisabledAtlas = "Menu_Main";
			// pm.onDisabledAtlasKey = "Button_Disabled";
			pm.size = {(this->_size[0] / 2 - 45), 50};
			pm.textColor = {0.7, 0.7, 0.7, 1.0};
			pm.textScale = 0.8f;

			pm.pos = {this->_pos[0] + 10, this->_pos[1] + this->_size[1] - 75};
			pm.content = "Confirm";

			auto btn = std::make_unique<Buttons::ColoredButton>(pm);

			this->AddElement("BTN_Confirm", std::move(btn), 2);

			pm.pos = {(this->_pos[0] + this->_size[0] / 2 + 35), this->_pos[1] + this->_size[1] - 75};
			pm.content = "Cancel";

			btn = std::make_unique<Buttons::ColoredButton>(pm);

			this->AddElement("BTN_Cancel", std::move(btn), 2);
		}
	}

	void WorldDeletionCheck::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->GetElement<Image>("IMG_BG")->SetPos(this->_pos);
		auto txt = this->GetElement<Text>("TXT_Confirm");

		txt->SetPos({this->_pos[0] + this->_size[0] / 2 - txt->GetTextSize()[0] / 2, this->_pos[1] + 35});

		txt = this->GetElement<Text>("TXT_WN");
		txt->SetPos({this->_pos[0] + this->_size[0] / 2 - txt->GetTextSize()[0] / 2, this->_pos[1] + 90});
	}

	void WorldDeletionCheck::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->GetElement<Image>("IMG_BG")->SetSize(this->_size);

		auto txt = this->GetElement<Text>("TXT_Confirm");

		txt->SetPos({this->_pos[0] + this->_size[0] / 2 - txt->GetTextSize()[0] / 2, this->_pos[1] + 35});

		txt = this->GetElement<Text>("TXT_WN");
		txt->SetPos({this->_pos[0] + this->_size[0] / 2 - txt->GetTextSize()[0] / 2, this->_pos[1] + 90});
	}
} // namespace Vox::Game::Scenes::Menu::Interfaces::Elements