#include "Game/Scenes/Menu/I_MenuCreate.hpp"

#include "MathGraphicalLib/Utils.hpp"

#include "Game/Scenes/Menu/SavesData.hpp"

#include "Front/Interfaces/Elements/Buttons/TexturedButton.hpp"
#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/InputField.hpp"
#include "Front/Interfaces/Elements/Text.hpp"
#include "Front/Interfaces/InterfacesManager.hpp"

#include <GLFW/glfw3.h>

#include "Game/GameManager.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	using namespace Front::Interfaces::Elements;

	I_MenuCreate::I_MenuCreate(Vox::Front::Interfaces::Elements::Vector2 pos,
							   Vox::Front::Interfaces::Elements::Vector2 size)
		: AInterface(pos, size, false)
	{
		_size[0] = MGL::Utils::findNextMultiple(size[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(size[1], 16.f);

		this->AddElement("IMG_BG",
						 std::make_unique<Image>("Menu_Main", "Dirt", this->_pos, this->_size,
												 Color(0.3f, 0.3f, 0.3f, 1.0f),
												 Vox::Front::Interfaces::Elements::Vector2(16, 16)),
						 0);

		{
			Text::Vox_Text_Constructor st{};
			st.content = "Create New World";
			st.scale = 1.5f;
			st.pos = {this->_pos[0] + this->_size[0] / 2 - Text::GetTextSize(st.content, st.scale)[0] / 2,
					  this->_pos[1] + 100 + 25};
			st.size = {Text::GetTextSize(st.content, st.scale)[0], 50};

			this->AddElement("TXT_PageName", std::make_unique<Text>(st), 1);
		}

		{ // IF_Name, IF_Seed
			InputField::Constructor st{};
			st.BGAtlas = "Menu_Main";
			st.BGAtlasKey = "Dirt";
			st.inputMode = InputField::E_InputMode::AlphaNumeric;
			st.pos = {this->_pos[0] + this->_size[0] / 2 + 50, this->_pos[1] + 250};
			st.size = {400, 50};

			auto inputF = std::make_unique<InputField>(st);

			inputF->onTextChange.AddCallBack(
				[this](const std::string &newText)
				{
					auto btn = this->GetElement<Buttons::TexturedButton>("BTN_Create");
					if (newText.size() == 0 && btn->IsEnabled() == true)
						btn->ChangeEnableState(false);
					else if (newText.size() != 0 && btn->IsEnabled() == false)
						btn->ChangeEnableState(true);
				});

			this->AddElement("IF_Name", std::move(inputF), 1);

			Text::Vox_Text_Constructor st2{};
			st2.content = "World Name:";
			st2.scale = 0.7f;
			st2.pos = {this->_pos[0] + this->_size[0] / 2 - 50 - Text::GetTextSize(st2.content, st2.scale)[0],
					   this->_pos[1] + 250 + 25};
			st2.size = {Text::GetTextSize(st2.content, st2.scale)[0], 50};

			this->AddElement("TXT_Name", std::make_unique<Text>(st2), 1);

			st.inputMode = InputField::E_InputMode::Numeric;
			st.pos[1] += 100;

			this->AddElement("IF_Seed", std::make_unique<InputField>(st), 1);

			st2.content = "Seed:";
			st2.pos = {this->_pos[0] + this->_size[0] / 2 - 50 - Text::GetTextSize(st2.content, st2.scale)[0],
					   st.pos[1] + 25};
			st2.size = {Text::GetTextSize(st2.content, st2.scale)[0], 50};

			this->AddElement("TXT_Seed", std::make_unique<Text>(st2), 1);
		}

		{
			Buttons::TexturedButton::Vox_TexturedButton_Constructor pm{};
			pm.atlas = "Menu_Main";
			pm.atlasKey = "Button";
			pm.hoverTXTColor = {1.0, 1.0, 1.0, 1.0};
			pm.onHoverAtlas = "Menu_Main";
			pm.onHoverAtlasKey = "Button_Hover";
			pm.disabledTXTColor = {0.5, 0.5, 0.5, 1.0};
			pm.onDisabledAtlas = "Menu_Main";
			pm.onDisabledAtlasKey = "Button_Disabled";
			pm.size = {(this->_size[0] - this->_size[0] / 4) / 2 - 50, 75};
			pm.textColor = {0.7, 0.7, 0.7, 1.0};
			pm.textScale = 0.8f;

			pm.content = "Cancel";
			pm.pos = {this->_pos[0] + this->_size[0] / 2 - pm.size[0] - 100,
					  this->_pos[1] + this->_size[1] - this->_size[1] / 4};

			auto btn = std::make_unique<Buttons::TexturedButton>(pm);

			btn->onClickCallbacks.AddCallBack(
				[this](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;
					this->GetElement<InputField>("IF_Name")->SetValue("");
					this->GetElement<InputField>("IF_Seed")->SetValue("");
					auto &im = Front::Interfaces::InterfacesManager::GetInstance();
					im.DisableInterface("Create");
					im.EnableInterface("World");
				});

			this->AddElement("BTN_Cancel", std::move(btn), 1);

			pm.content = "Validate";
			pm.pos[0] = this->_pos[0] + this->_size[0] / 2 + 100;

			btn = std::make_unique<Buttons::TexturedButton>(pm);

			btn->onClickCallbacks.AddCallBack(
				[this](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;
					std::string worldName = this->GetElement<InputField>("IF_Name")->GetValue();
					std::string worldSeed = this->GetElement<InputField>("IF_Seed")->GetValue();

					Saves::WorldData wd = Saves::CreateSaveData(worldName, worldSeed);
					if (wd.worldName == "")
						return;
					if (Saves::SaveWorldData(wd) == false)
						return ;
					Game::GameManager::GetInstance().SetSaveData(wd);
					Game::GameManager::GetInstance().GetSceneManager().LoadScene("World");
				});
			btn->ChangeEnableState(false);

			this->AddElement("BTN_Create", std::move(btn), 1);
		}
	}

	void I_MenuCreate::SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->GetElement<Image>("IMG_BG")->SetPos(this->_pos);
	}

	void I_MenuCreate::SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		_size[0] = MGL::Utils::findNextMultiple(newSize[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(newSize[1], 16.f);
		this->GetElement<Image>("IMG_BG")->SetSize(this->_size);
	}

	void I_MenuCreate::Render()
	{
		for (auto &elem : this->_content)
			if (elem.elem)
				elem.elem->Draw();
	}
} // namespace Vox::Game::Scenes::Menu::Interfaces