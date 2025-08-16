#include "Game/Scenes/Menu/I_MenuMain.hpp"

#include "Front/Interfaces/Elements/Image.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	I_MenuMain::I_MenuMain(Vector2 pos, Vector2 size) : AInterface(pos, size)
	{
		this->AddElement(
			"IMG_Front",
			std::make_unique<Front::Interfaces::Elements::Image>(Vector2{100, 100}, Vector2{this->_size[0] - 200, this->_size[1] - 200}),
			0);
	}

	I_MenuMain::~I_MenuMain() {}

	void I_MenuMain::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->FindElement("IMG_Front")->elem->SetPos({this->_pos[0] + 100, this->_pos[1] + 100});
	}

	void I_MenuMain::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->FindElement("IMG_Front")->elem->SetSize({this->_size[0] - 200, this->_size[1] - 200});
	}
	
	void I_MenuMain::Render()
	{
		for (auto &elem : this->_content)
			if (elem.elem)
				elem.elem->Draw();

	}
} // namespace Vox::Front::Interfaces