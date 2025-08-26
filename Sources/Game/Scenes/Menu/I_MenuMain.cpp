#include "Game/Scenes/Menu/I_MenuMain.hpp"

#include "Front/Interfaces/Elements/Buttons/ColoredButton.hpp"

#include "Front/Interfaces/Elements/Buttons/TexturedButton.hpp"
#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include "MathGraphicalLib/Utils.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	using namespace Front::Interfaces::Elements;

	I_MenuMain::I_MenuMain(Vector2 pos, Vector2 size) : AInterface(pos, size)
	{
		_size[0] = MGL::Utils::findNextMultiple(size[0], (size_t)16);
		_size[1] = MGL::Utils::findNextMultiple(size[1], (size_t)16);
		// std::cout << size << "\n" << std::endl;
		this->AddElement("IMG_BG",
						 std::make_unique<Image>("Menu_Main", "Dirt", this->_pos, this->_size, Color(1.0f, 1.0f, 1.0f, 1.0f),
												 Vector2(16,16)),
						 0);


		// {
		// 	Text::Vox_Text_Constructor params{};
		// 	params.pos = {100, 100};
		// 	params.content = "Test";
		// 	params.color = {0.0f, 1.0f, 0.0f, 1.0f};

		// 	this->AddElement("TXT_Test", std::make_unique<Text>(params), 1);
		// }
		// {
		// 	Buttons::ColoredButton::Vox_ColorButton_Constructor params{};
		// 	params.pos = {600, 200};
		// 	params.content = "Test";
		// 	params.size = {100, 50};
		// 	params.bgColor = {0.0, 0.0, 1.0, 1.0};
		// 	params.hoverBGColor = {1.0, 0.0, 1.0, 1.0};
		// 	params.textColor = {0.0, 1.0, 1.0, 1.0};
		// 	params.hoverTXTColor = {1.0, 1.0, 1.0, 1.0};

		// 	this->AddElement("BTN_Test", std::make_unique<Buttons::ColoredButton>(params), 1);
		// }

		// {
		// 	Buttons::TexturedButton::Vox_TexturedButton_Constructor params{};
		// 	params.pos = {400, 200};
		// 	params.content = "Test";
		// 	params.size = {100, 50};
		// 	params.atlas = "Menu_Main";
		// 	params.atlasKey = "Button";
		// 	params.textColor = {0.0, 0.0, 1.0, 1.0};
		// 	params.hoverTXTColor = {1.0, 1.0, 1.0, 1.0};
		// 	params.onHoverAtlas = "Menu_Main";
		// 	params.onHoverAtlasKey = "Button_Hover";

		// 	this->AddElement("BTN_Test1", std::make_unique<Buttons::TexturedButton>(params), 1);
		// }
	}

	I_MenuMain::~I_MenuMain() {}

	void I_MenuMain::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->FindElement("IMG_BG")->elem->SetPos(this->_pos);
		// this->FindElement("TXT_Test")->elem->SetPos({100, 100});
		// this->FindElement("BTN_Test1")->elem->SetPos({400, 200});
		// this->FindElement("BTN_Test")->elem->SetPos({600, 200});
	}

	void I_MenuMain::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->FindElement("IMG_BG")->elem->SetSize(this->_size);
		// this->FindElement("TXT_Test")->elem->SetSize({this->_size[0] - 200, this->_size[1] - 200});
	}

	void I_MenuMain::Render()
	{
		for (auto &elem : this->_content)
			if (elem.elem)
				elem.elem->Draw();
		// std::cout << "render interface" << std::endl;
	}
} // namespace Vox::Game::Scenes::Menu::Interfaces