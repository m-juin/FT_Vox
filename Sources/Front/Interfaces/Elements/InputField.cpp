#include "Front/Interfaces/Elements/InputField.hpp"

#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include <GLFW/glfw3.h>

#include "Game/Utils/Datas/E_InputMask.hpp"

namespace Vox::Front::Interfaces::Elements
{

	InputField::InputField(const Constructor &st) : AElement(st.pos, st.size), AContainer(), AFocusable(Game::Utils::Datas::InputMask::KeyBoard), _inputScale(st.inputScale), _inputMode(st.inputMode)
	{
		Color BGColor{1.0, 1.0, 1.0, 1.0};
		if (st.BGAtlas == "")
			BGColor[3] = 0.2f;
		this->AddElement("IMG_BG", std::make_unique<Image>(st.BGAtlas, st.BGAtlasKey, st.pos, st.size, BGColor), 0);
		{ // TXT_Input
			Text::Vox_Text_Constructor pm{};
			pm.size = this->_size;
			pm.pos = {this->_pos[0] + this->_size[0] / 2 - Text::GetTextSize(st.defaultValue, st.inputScale)[0] / 2,
					  this->_pos[1] + this->_size[1] / 2};
			pm.content = st.defaultValue;
			pm.scale = st.inputScale;
			pm.color = {1., 1., 1., 1.};
			pm.letterSpace = 5;

			this->AddElement("TXT_Input", std::make_unique<Text>(pm), 1);
		}

		this->AddElement("IMG_PosIndicator",
						 std::make_unique<Image>(
							 "", "", Vector2(this->_pos[0] + this->_size[0] / 2., this->_pos[1] + this->_size[1] / 4.),
							 Vector2(3., this->_size[1] / 2)),
						 2);
	}

	void InputField::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->GetElement<Image>("IMG_BG")->SetPos(this->_pos);
	}

	const std::string &InputField::GetValue()
	{
		return this->GetElement<Text>("TXT_Input")->GetContent();
	}

	void InputField::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->GetElement<Image>("IMG_BG")->SetSize(this->_size);
	}

	void InputField::SetValue(const std::string &newVal)
	{
		auto txt = this->GetElement<Text>("TXT_Input");
		if (newVal == txt->GetContent())
			return;
		txt->SetContent(newVal);
	}

	void InputField::Draw()
	{
		for (auto &cElem : this->_content)
		{
			if (cElem.key != "IMG_PosIndicator")
				cElem.elem->Draw();
			else if (this->_isFocused && cElem.key == "IMG_PosIndicator")
			{
				if (this->drawCount > 15)
				{
					drawCount += 1;
					if (drawCount > 30)
						this->drawCount = 0;
					cElem.elem->Draw();
				}
				else
					this->drawCount += 1;
			}
		}
	}

	void InputField::OnClick(const int &button, const int &action)
	{
		if (this->currentlyHovered == false)
			return;
		AClickable::OnClick(button, action);
		AFocusable::SetFocusElement(this);
		this->curPos = this->GetElement<Text>("TXT_Input")->GetContent().size();
		this->UpdateIndicatorPos();
		this->drawCount = 0;
	}

	void InputField::HandleCharInput(const unsigned int &code)
	{
		char converted = code;

		int im = (int)this->_inputMode;

		bool isValid = im == 4	 ? std::isprint(converted)
					   : im == 2 ? std::isalnum(converted)
					   : im == 1 ? std::isdigit(converted)
								 : std::isalpha(converted);

		if (isValid)
		{
			auto elem = this->GetElement<Text>("TXT_Input");
			auto tc = elem->GetContent();
			auto it = tc.begin();
			std::advance(it, curPos);
			tc.insert(it, 1, converted);
			elem->SetPos({this->_pos[0] + this->_size[0] / 2 - Text::GetTextSize(tc, this->_inputScale)[0] / 2,
						  this->_pos[1] + this->_size[1] / 2});
			elem->SetContent(tc);
			curPos += 1;
			UpdateIndicatorPos();
			this->onTextChange.Notify(this->GetValue());
		}
	}

	void InputField::UpdateIndicatorPos()
	{
		auto indicator = this->GetElement<Image>("IMG_PosIndicator");

		auto txt = this->GetElement<Text>("TXT_Input");

		if (txt->GetContent().size() == 0)
			indicator->SetPos(Vector2(this->_pos[0] + this->_size[0] / 2., this->_pos[1] + this->_size[1] / 4.));
		else
		{
			auto txtSize = Text::GetTextSize(txt->GetContent().substr(0, curPos), this->_inputScale, 5);

			Vector2 newPos = {txt->GetPos()[0] + txtSize[0] - 2, indicator->GetPos()[1]};
			indicator->SetPos(newPos);
		}
	}

	void InputField::HandleKeyInput(const unsigned int &code, const int &action)
	{
		auto elem = this->GetElement<Text>("TXT_Input");
		auto tc = elem->GetContent();
		if (code == GLFW_KEY_BACKSPACE && action != GLFW_RELEASE)
		{
			if (tc.size() == 0 || curPos == 0)
				return;
			std::string tmp = "";
			if (curPos != 1)
				tmp += tc.substr(0, curPos - 1);
			tmp += tc.substr(curPos);
			elem->SetPos({this->_pos[0] + this->_size[0] / 2 - Text::GetTextSize(tmp, this->_inputScale)[0] / 2,
						  this->_pos[1] + this->_size[1] / 2});
			elem->SetContent(tmp);
			curPos -= 1;
			UpdateIndicatorPos();
			this->onTextChange.Notify(this->GetValue());
		}
		else if (code == GLFW_KEY_DELETE && action != GLFW_RELEASE)
		{
			if (tc.empty() || curPos >= tc.size())
				return;

			std::string tmp = tc.substr(0, curPos);
			tmp += tc.substr(curPos + 1);

			elem->SetPos({this->_pos[0] + this->_size[0] / 2 - Text::GetTextSize(tmp, this->_inputScale)[0] / 2,
						  this->_pos[1] + this->_size[1] / 2});
			elem->SetContent(tmp);
			UpdateIndicatorPos();
			this->onTextChange.Notify(this->GetValue());
		}
		else if (code == GLFW_KEY_LEFT && action != GLFW_RELEASE)
		{
			if (curPos == 0 || tc.empty())
				return;
			curPos -= 1;
			UpdateIndicatorPos();
		}
		else if (code == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
		{
			if (curPos >= tc.size() || tc.empty())
				return;
			curPos += 1;
			UpdateIndicatorPos();
		}
		else if ((code == GLFW_KEY_ESCAPE || code == GLFW_KEY_ENTER) && action == GLFW_RELEASE)
		{
			AFocusable::_focusedElement = nullptr;
			this->_isFocused = false;
			AFocusable::SetFocusElement(nullptr);
		}
	}
} // namespace Vox::Front::Interfaces::Elements