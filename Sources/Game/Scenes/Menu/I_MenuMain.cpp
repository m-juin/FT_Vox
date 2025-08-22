#include "Game/Scenes/Menu/I_MenuMain.hpp"

#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	using namespace Front::Interfaces::Elements;

	I_MenuMain::I_MenuMain(Vector2 pos, Vector2 size) : AInterface(pos, size)
	{
		std::cout << size << "\n" << std::endl;
		this->AddElement(
			"IMG_Front",
			std::make_unique<Image>(
				"Menu_Main", "Stone", Vector2{this->_size[0] / 2 - 128, this->_size[1] / 2 - 128}, Vector2{256, 256}),
			0);

		Text::Vox_Text_Constructor params{};
		params.pos = {100, 100};
		params.content = "Test";
		params.color = {0.0f, 1.0f, 0.0f, 1.0f};
		
		this->AddElement(
			"TXT_Test",
			std::make_unique<Text>(params),
			1);
	}

	I_MenuMain::~I_MenuMain() {}

	void I_MenuMain::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->FindElement("IMG_Front")->elem->SetPos({this->_pos[0] + 100, this->_pos[1] + 100});
		this->FindElement("TXT_Test")->elem->SetPos({100, 100});
	}

	void I_MenuMain::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->FindElement("IMG_Front")->elem->SetSize({this->_size[0] - 200, this->_size[1] - 200});
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