
#include "Game/Scenes/World/Interfaces/I_F3.hpp"

#include "Front/Interfaces/Elements/Text.hpp"

#include "MathGraphicalLib/Utils.hpp"

#include "Game/Scenes/World/WorldManager.hpp"

#include <sstream>

#include "Game/Scenes/World/Generation/BiomesPerlin.hpp"

namespace Vox::Game::Scenes::World::Interfaces
{
	using namespace Front::Interfaces::Elements;
	I_F3::I_F3(Vox::Front::Interfaces::Elements::Vector2 pos, Vox::Front::Interfaces::Elements::Vector2 size)
		: AInterface(pos, size), AUpdatable(2)
	{
		{
			Text::Vox_Text_Constructor pm{};
			pm.color = {0.5, 0.5, 0.5, 1.0};
			pm.content = "Player pos: ";
			pm.scale = 0.3f;
			pm.pos = {this->_pos[0] + 50, this->_pos[1] + 50};
			pm.size = {100, 50};

			this->AddElement("TXT_PlayerCoord", std::make_unique<Text>(pm));
		}
		{
			Text::Vox_Text_Constructor pm{};
			pm.color = {0.5, 0.5, 0.5, 1.0};
			pm.content = "LoadedChunck: ";
			pm.scale = 0.3f;
			pm.pos = {this->_pos[0] + 50, this->_pos[1] + 75};
			pm.size = {100, 50};

			this->AddElement("TXT_C_Loaded", std::make_unique<Text>(pm));

			pm.content = "ChunckWaiting: ";
			pm.pos = {this->_pos[0] + 50, this->_pos[1] + 100};
			this->AddElement("TXT_C_Waiting", std::make_unique<Text>(pm));
		}

		{
			Text::Vox_Text_Constructor pm{};
			pm.color = {0.5, 0.5, 0.5, 1.0};
			pm.content = "Generation Datas:";
			pm.scale = 0.3;
			pm.pos = {this->_pos[0] + 50, this->_pos[1] + 150};
			pm.size = {100, 50};
			this->AddElement("TXT_Generation_Label", std::make_unique<Text>(pm));
			pm.pos[0] += 50;
			pm.pos[1] += 25;
			pm.content = "Continental: ";
			this->AddElement("TXT_Generation_Continental", std::make_unique<Text>(pm));
			pm.pos[1] += 25;
			pm.content = "Erosion: ";
			this->AddElement("TXT_Generation_Erosion", std::make_unique<Text>(pm));
			pm.pos[1] += 25;
			pm.content = "P&V: ";
			this->AddElement("TXT_Generation_PV", std::make_unique<Text>(pm));
		}
		
		this->onUpdate.AddCallBack(
			[this]()
			{
				this->UpdatePlayerPos();
				this->UpdateGenerationCount();
				this->UpdateBiomeInfos();
			});
	}

	void I_F3::UpdateBiomeInfos()
	{
		auto &wm = Game::World::WorldManager::GetInstance();
		auto info = Game::Generation::Perlins::GetBiomeInfoAtPoint(wm.GetCamera().GetPosition()[0], wm.GetCamera().GetPosition()[2], wm.GetGenerationManager().GetSeed());
		std::stringstream ss;

		ss << "Continental: " << info.Continental;
		this->GetElement<Text>("TXT_Generation_Continental")->SetContent(ss.str());
		ss.clear();
		ss.str("");

		ss << "Erosion: " << info.Erosion;
		this->GetElement<Text>("TXT_Generation_Erosion")->SetContent(ss.str());
		ss.clear();
		ss.str("");

		ss << "P&V: " << info.PV;
		this->GetElement<Text>("TXT_Generation_PV")->SetContent(ss.str());
		ss.clear();
		ss.str("");
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
	}

	void I_F3::Render()
	{
		for (auto &elem : this->_content)
		{
			if (elem.elem && elem.isVisible)
			{
				elem.elem->Draw();
			}
		}
	}

	void I_F3::UpdatePlayerPos()
	{
		auto pPos = Game::World::WorldManager::GetInstance().GetCamera().GetPosition();
		std::stringstream ss;
		ss << "Player pos: x: " << pPos[0] << ", y: " << pPos[1] << ", z: " << pPos[2];
		this->GetElement<Text>("TXT_PlayerCoord")->SetContent(ss.str());
	}

	void I_F3::UpdateGenerationCount()
	{
		auto &wm = Game::World::WorldManager::GetInstance();
		size_t waiting = wm.GetGenerationManager().GetWaitingData();
		size_t loaded = wm.GetLoadedChunckData();

		std::stringstream ss; 
		ss << "Loaded Chunck: " << loaded;
		this->GetElement<Text>("TXT_C_Loaded")->SetContent(ss.str());
		ss.str("");
		ss << "Waiting Chunck: " << waiting;
		this->GetElement<Text>("TXT_C_Waiting")->SetContent(ss.str());
	}
} // namespace Vox::Game::Scenes::World::Interfaces