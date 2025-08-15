#include "Front/Interfaces/Menu.hpp"

#include "Front/Interfaces/Elements/Image.hpp"

namespace Vox::Front::Interfaces
{
	Menu::Menu(Vector2 pos, Vector2 size) : AInterface(pos, size)
	{
		this->AddElement(
			"IMG_Front",
			std::make_unique<Elements::Image>(Vector2{100, 100}, Vector2{this->_size[0] - 200, this->_size[1] - 200}),
			0);
	}

	Menu::~Menu() {}

	void Menu::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->FindElement("IMG_Front")->elem->SetPos({this->_pos[0] + 100, this->_pos[1] + 100});
	}

	void Menu::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->FindElement("IMG_Front")->elem->SetSize({this->_size[0] - 200, this->_size[1] - 200});
	}
} // namespace Vox::Front::Interfaces