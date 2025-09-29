#include "Front/Interfaces/Elements/Slider.hpp"

#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include "GLFW/glfw3.h"

#include "Front/Window.hpp"

#include <iomanip>
#include <sstream>

#include "Game/Utils/Datas/E_InputMask.hpp"

namespace Vox::Front::Interfaces::Elements
{
	void Slider::SetMinVal(const float &newMin)
	{
		if (newMin == this->_range.first)
			return;
		else if (newMin >= this->_range.second)
			throw std::runtime_error("Invalid minimum slider value.");
		this->_range.first = newMin;
		this->UpdateVals();
	}
	void Slider::SetMaxVal(const float &newMax)
	{
		if (newMax == this->_range.second)
			return;
		else if (newMax <= this->_range.first)
			throw std::runtime_error("Invalid maximum slider value.");
		this->_range.second = newMax;
		this->UpdateVals();
	}
	void Slider::SetStep(const float &newStep)
	{
		if (newStep == this->_step)
			return;
		else if (newStep == 0.f || newStep > this->_range.second - this->_range.first)
			throw std::runtime_error("Invalid slider step value.");
		this->_step = newStep;
	}
	void Slider::SetValue(const float &newVal)
	{
		if (newVal == this->_value)
			return;
		else if (newVal < this->_range.first || newVal > this->_range.second)
		{
			// std::cout << newVal << std::endl;
			throw std::runtime_error("Invalid slider value.");
		}
		this->_value = newVal;
		this->GetElement<Image>("IMG_Handle")->SetPos(Vector2(this->GetHandlePos(), this->_pos[1]));
		std::stringstream ss;
		ss << std::setprecision(2) << this->_value;
		this->GetElement<Text>("TXT_Value")->SetContent(ss.str());

	}

	void Slider::UpdateVals()
	{
		return;
	}

	float Slider::GetHandlePos()
	{
		float initial = this->_pos[0] - (this->_size[1] / 2);

		float normalizedVal = (this->_value - this->_range.first) / this->_range.second - this->_range.first;

		initial += this->_size[0] * normalizedVal;
		return initial;
	}

	void Slider::OnClick(const int &button, const int &action)
	{
		if (button != GLFW_MOUSE_BUTTON_LEFT)
			return;

		if (action == GLFW_RELEASE && this->_isFocused)
		{
			auto handle = this->GetElement<Image>("IMG_Handle");
			handle->SetColor(this->_handleColor);
			this->SetFocusElement(nullptr);
		}
		else if (action == GLFW_PRESS && this->_isFocused == false)
		{
			double x, y;
			auto handle = this->GetElement<Image>("IMG_Handle");
			glfwGetCursorPos(Window::GetInstance().GetWindow(), &x, &y);
			if ((x >= handle->GetPos()[0] && x <= handle->GetPos()[0] + handle->GetSize()[1]) &&
				(y >= handle->GetPos()[1] && y <= handle->GetPos()[1] + handle->GetSize()[1]))
			{
				handle->SetColor(this->_FocusedHandleColor);
				this->prevX = x;
				this->SetFocusElement(this);
			}
		}
	}

	void Slider::HandleMovement(float xPos)
	{
		if (xPos == this->prevX)
			return;

		float prevVal = this->_value;

		if (xPos <= this->_pos[0])
			this->_value = this->_range.first;
		else if (xPos >= this->_pos[0] + this->_size[0])
			this->_value = this->_range.second;
		else
		{
			float localPos = xPos - this->_pos[0];
			localPos /= this->_size[0];
			this->_value = this->_range.first + (this->_range.second - this->_range.first) * localPos;
		}
		prevX = xPos;
		this->_value = std::round(this->_value * 100.0f) / 100.0f;
		if (prevVal == this->_value)
			return;
		std::stringstream ss;
		ss << std::setprecision(2) << this->_value;
		this->GetElement<Text>("TXT_Value")->SetContent(ss.str());
		this->GetElement<Image>("IMG_Handle")->SetPos(Vector2(this->GetHandlePos(), this->_pos[1]));
		this->onValueChange.Notify(this->_value, prevVal);
	}

	bool Slider::IsHover(const Vector2 &mousePos)
	{
		return AClickable::IsHover(mousePos);
	}

	void Slider::SetPos(Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->GetElement<Image>("IMG_BG")->SetPos(Vector2(this->_pos[0], this->_pos[1] + this->_size[1] / 4));
		this->GetElement<Image>("IMG_Handle")->SetPos(Vector2(this->GetHandlePos(), this->_pos[1]));
		this->GetElement<Image>("TXT_Value")->SetPos({this->_pos[0] + this->_size[0] + 25, this->_pos[1] + 16});
	}

	void Slider::SetSize(Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->GetElement<Image>("IMG_BG")->SetSize(Vector2(this->_size[0], this->_size[1] / 2));
		this->GetElement<Image>("IMG_Handle")->SetSize(Vector2(this->_size[1] / 2));
	}

	Slider::Slider(const Constructor &st)
		: AElement(st.pos, st.size), AFocusable(Game::Utils::Datas::InputMask::Mouse), _value(st.defaultValue), _step(st.step), _range({st.minVal, st.maxVal}),
		  _handleColor(st.HandleColor), _FocusedHandleColor(st.HandleSlidingColor)
	{
		this->AddElement("IMG_BG",
						 std::make_unique<Image>("", "", Vector2(this->_pos[0], this->_pos[1] + this->_size[1] / 4),
												 Vector2(this->_size[0], this->_size[1] / 2), st.BGColor),
						 0);
		std::cout << this->_pos[0] + (this->_size[1] / 2) - GetHandlePos() << std::endl;

		this->AddElement("IMG_Handle",
						 std::make_unique<Image>("", "", Vector2(this->GetHandlePos(), this->_pos[1]),
												 Vector2(this->_size[1]), st.HandleColor),
						 1);

		{
			Text::Vox_Text_Constructor pm{};

			pm.color = {0.5, 0.5, 0.5, 1.0};
			std::stringstream ss;
			ss << std::setprecision(2) << this->_value;
			pm.content = ss.str();
			pm.pos = {this->_pos[0] + this->_size[0] + 25, this->_pos[1] + 16};
			pm.scale = 0.35f;

			this->AddElement("TXT_Value", std::make_unique<Text>(pm), 1);
		}
	}
} // namespace Vox::Front::Interfaces::Elements