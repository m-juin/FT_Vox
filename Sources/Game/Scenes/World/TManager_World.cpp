#include "Game/Scenes/World/TManager_World.hpp"
#include "Game/GameManager.hpp"
namespace Vox::Game::Scenes::World
{
	TManager_World::TManager_World()
	{
		this->CreateMap();
	}

	TManager_World::~TManager_World()
	{
		for (auto pair : this->_texturesMap)
			delete pair.second;
	}

	void TManager_World::CreateMap()
	{
		const std::string T_Path = Game::GameManager::GetInstance().GetTexturePackPath();
		this->_texturesMap["Menu_Main"] = new Front::Utils::TexturesAtlas(
			{
				{"Dirt", T_Path + "GUI/dirt.png"},
				{"Button", T_Path + "GUI/button.png"},
				{"Button_Hover", T_Path + "GUI/button_highlighted.png"},
				{"Button_Disabled", T_Path + "GUI/button_disabled.png"},
			},
			4);
	}
} // namespace Vox::Game::Scenes::World