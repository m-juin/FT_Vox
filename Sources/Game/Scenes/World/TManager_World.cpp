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
	}
} // namespace Vox::Game::Scenes::World