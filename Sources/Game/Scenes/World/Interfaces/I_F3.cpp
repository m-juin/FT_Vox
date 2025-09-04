
#include "Game/Scenes/World/Interfaces/I_F3.hpp"

#include "Front/Interfaces/Elements/Text.hpp"

#include "MathGraphicalLib/Utils.hpp"

namespace Vox::Game::Scenes::World::Interfaces
{
	using namespace Front::Interfaces::Elements;
	I_F3::I_F3(Vox::Front::Interfaces::Elements::Vector2 pos, Vox::Front::Interfaces::Elements::Vector2 size)
		: AInterface(pos, size)
	{
		{
			Text::Vox_Text_Constructor pm{};
			pm.content = "Player pos: ";
			pm.pos = {this->_pos[0] + 100, this->_pos[1] + 100};
			pm.size = {100, 50};

			this->AddElement("TXT_PlayerCoord", std::make_unique<Text>(pm));
		}
	}

	I_F3::~I_F3() {}

	void I_F3::SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->FindElement("TXT_PlayerCoord")->elem->SetPos({this->_pos[0] + 100, this->_pos[1] + 100});
	}

	void I_F3::SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		_size[0] = MGL::Utils::findNextMultiple(newSize[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(newSize[1], 16.f);
		this->FindElement("IMG_BG")->elem->SetSize(this->_size);
	}

	void I_F3::Render()
	{
		for (auto &elem : this->_content)
			if (elem.elem)
				elem.elem->Draw();
	}
} // namespace Vox::Game::Scenes::World::Interfaces