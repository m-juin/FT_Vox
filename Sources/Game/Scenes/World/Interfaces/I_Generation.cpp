
#include "Game/Scenes/World/Interfaces/I_Generation.hpp"

#include "Front/Interfaces/Elements/Text.hpp"

#include "MathGraphicalLib/Utils.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

#include <sstream>

namespace Vox::Game::Scenes::World::Interfaces
{
	using namespace Front::Interfaces::Elements;
	I_Generation::I_Generation(Vox::Front::Interfaces::Elements::Vector2 pos, Vox::Front::Interfaces::Elements::Vector2 size)
		: AInterface(pos, size), AUpdatable(10)
	{
		{
			Text::Vox_Text_Constructor pm{};
			pm.color = {0.5, 0.5, 0.5, 1.0};
			pm.content = "Seed: ";
			pm.scale = 0.3f;
			pm.pos = {this->_pos[0] + 500, this->_pos[1] + 50};
			pm.size = {100, 50};

			this->AddElement("TXT_Seed", std::make_unique<Text>(pm));
		}
		{
		}

		this->onUpdate.AddCallBack(
			[this]()
			{
				this->UpdateSeed();
			});
		this->ChangeEnableState(false);
	}

	I_Generation::~I_Generation() {}

	void I_Generation::SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->FindElement("TXT_Seed")->elem->SetPos({this->_pos[0] + 100, this->_pos[1] + 100});
	}

	void I_Generation::SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		_size[0] = MGL::Utils::findNextMultiple(newSize[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(newSize[1], 16.f);
	}

	void I_Generation::Render()
	{
		for (auto &elem : this->_content)
			if (elem.elem)
				elem.elem->Draw();
	}

	void I_Generation::UpdateSeed()
	{
		auto seed = Game::World::WorldManager::GetInstance().GetGenerationManager().GetSeed();
		std::stringstream ss;
		ss << "Seed: " << seed;
		this->GetElement<Text>("TXT_Seed")->SetContent(ss.str());
	}
} // namespace Vox::Game::Scenes::World::Interfaces