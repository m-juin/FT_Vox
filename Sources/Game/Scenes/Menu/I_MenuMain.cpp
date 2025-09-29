#include "Game/Scenes/Menu/I_MenuMain.hpp"

#include "Front/Interfaces/Elements/Buttons/ColoredButton.hpp"
#include "Front/Interfaces/Elements/Buttons/TexturedButton.hpp"
#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"

#include "MathGraphicalLib/Utils.hpp"

#include <GLFW/glfw3.h>

#include "Front/Window.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	using namespace Front::Interfaces::Elements;

	I_MenuMain::I_MenuMain(Vox::Front::Interfaces::Elements::Vector2 pos, Vox::Front::Interfaces::Elements::Vector2 size) : AInterface(pos, size, true)
	{
		_size[0] = MGL::Utils::findNextMultiple(size[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(size[1], 16.f);
		this->_inputMask |= Game::Utils::Datas::InputMask::Mouse | Game::Utils::Datas::InputMask::KeyBoard;
		// std::cout << size << "\n" << std::endl;
		this->AddElement("IMG_BG",
						 std::make_unique<Image>("Menu_Main", "Dirt", this->_pos, this->_size,
												 Color(1.0f, 1.0f, 1.0f, 1.0f), Vox::Front::Interfaces::Elements::Vector2(16, 16)),
						 0);

		{
			Buttons::TexturedButton::Vox_TexturedButton_Constructor params{};
			params.size = {400, 75};
			params.pos = {this->_size[0] / 2 - params.size[0] / 2,
						  this->_size[1] / 2 - params.size[1] / 2 - this->_size[1] / 4};
			params.content = "Worlds";
			params.atlas = "Menu_Main";
			params.atlasKey = "Button";
			params.textColor = {0.0, 0.0, 1.0, 1.0};
			params.hoverTXTColor = {1.0, 1.0, 1.0, 1.0};
			params.onHoverAtlas = "Menu_Main";
			params.onHoverAtlasKey = "Button_Hover";
			params.textScale = 0.8f;
			//
			this->AddElement("BTN_Worlds", std::make_unique<Buttons::TexturedButton>(params), 1);

			params.content = "Options";
			params.pos[1] += this->_size[1] / 4;

			this->AddElement("BTN_Options", std::make_unique<Buttons::TexturedButton>(params), 1);

			params.content = "Quit";
			params.pos[1] += this->_size[1] / 4;

			this->AddElement("BTN_Quit", std::make_unique<Buttons::TexturedButton>(params), 1);

			this->GetElement<Buttons::TexturedButton>("BTN_Quit")
				->onClickCallbacks.AddCallBack(
					[](const int &button, const int &action)
					{
						if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
							return;
						Front::Window::GetInstance().RequestEnd();
					});

			this->GetElement<Buttons::TexturedButton>("BTN_Worlds")
				->onClickCallbacks.AddCallBack(
					[](const int &button, const int &action)
					{
						if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
							return;
						auto &im = Front::Interfaces::InterfacesManager::GetInstance();
						im.EnableInterface("World");
						im.DisableInterface("Main");
					});
			
		}
	}

	I_MenuMain::~I_MenuMain() {}

	void I_MenuMain::SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->FindElement("IMG_BG")->elem->SetPos(this->_pos);
		// this->FindElement("TXT_Test")->elem->SetPos({100, 100});
		// this->FindElement("BTN_Test1")->elem->SetPos({400, 200});
		// this->FindElement("BTN_Test")->elem->SetPos({600, 200});
	}

	void I_MenuMain::SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		_size[0] = MGL::Utils::findNextMultiple(newSize[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(newSize[1], 16.f);
		this->FindElement("IMG_BG")->elem->SetSize(this->_size);
		// this->FindElement("TXT_Test")->elem->SetSize({this->_size[0] - 200, this->_size[1] - 200});
	}

	void I_MenuMain::Render()
	{
		for (auto &elem : this->_content)
		{
			if (elem.elem && elem.isVisible)
			{
				elem.elem->Draw();
			}
		}
	}
} // namespace Vox::Game::Scenes::Menu::Interfaces