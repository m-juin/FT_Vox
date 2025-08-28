#include "Game/Scenes/Menu/I_MenuWorld.hpp"

#include "Front/Interfaces/Elements/Buttons/ColoredButton.hpp"
#include "Front/Interfaces/Elements/Buttons/TexturedButton.hpp"
#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"
#include "Front/Interfaces/Elements/ScrollableList.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"

#include "MathGraphicalLib/Utils.hpp"

#include <GLFW/glfw3.h>

#include "Front/Window.hpp"

#include <vector>

#include "Game/Scenes/Menu/SavesData.hpp"

#include "Game/Scenes/Menu/InterfacesElements/WorldDataDisplayer.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	using namespace Front::Interfaces::Elements;

	std::vector<Saves::WorldData> LoadGamesDatas()
	{
		std::vector<Saves::WorldData> lst;

		const std::string path = Saves::WorldsFolder;

		for (const auto &entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory() == false) continue;

			std::filesystem::path saveFile(entry.path() / "SaveData.json");
			if (std::filesystem::is_regular_file(saveFile) == false) continue;
			auto wd = Saves::LoadWorldData(saveFile);

			if (wd.folderPath == "") continue;
			lst.push_back(wd);
		}
		return lst;
	}

	I_MenuWorld::I_MenuWorld(Vox::Front::Interfaces::Elements::Vector2 pos, Vox::Front::Interfaces::Elements::Vector2 size) : AInterface(pos, size, false)
	{
		_size[0] = MGL::Utils::findNextMultiple(size[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(size[1], 16.f);
		// std::cout << size << "\n" << std::endl;
		this->AddElement("IMG_BG",
						 std::make_unique<Image>("Menu_Main", "Dirt", this->_pos, this->_size,
												 Color(0.3f, 0.3f, 0.3f, 1.0f), Vox::Front::Interfaces::Elements::Vector2(16, 16)),
						 0);
		{ // TXT_Choose
			Text::Vox_Text_Constructor pm{};
			pm.color = {0.8, 0.8, 0.8, 1.0};
			pm.scale = 0.8f;
			pm.content = "Select your world";
			pm.pos = {this->_pos[0] + this->_size[0] / 2 - (Text::GetTextSize(pm.content, pm.scale)[0] / 2), 100};
			pm.size = {200, 25};

			this->AddElement("TXT_Choose", std::make_unique<Text>(pm), 1);
		};

		{ // BTN Join + BTN Create + BTN_Cancel
			Buttons::TexturedButton::Vox_TexturedButton_Constructor pm{};
			pm.atlas = "Menu_Main";
			pm.atlasKey = "Button";
			pm.hoverTXTColor = {1.0, 1.0, 1.0, 1.0};
			pm.onHoverAtlas = "Menu_Main";
			pm.onHoverAtlasKey = "Button_Hover";
			pm.disabledTXTColor = {0.5, 0.5, 0.5, 1.0};
			pm.onDisabledAtlas = "Menu_Main";
			pm.onDisabledAtlasKey = "Button_Disabled";
			pm.size = {(this->_size[0] - this->_size[0] / 4) / 2 - 50, 75};
			pm.textColor = {0.7, 0.7, 0.7, 1.0};
			pm.textScale = 0.8f;

			pm.content = "Join World";
			pm.pos = {this->_pos[0] + this->_size[0] / 2 - pm.size[0] - 100,
					  this->_pos[1] + this->_size[1] - this->_size[1] / 4};

			this->AddElement("BTN_Join", std::make_unique<Buttons::TexturedButton>(pm), 1);

			pm.content = "Create new World";
			pm.pos[0] = this->_pos[0] + this->_size[0] / 2 + 100;

			this->AddElement("BTN_Create", std::make_unique<Buttons::TexturedButton>(pm), 1);

			pm.content = "Cancel";
			pm.size[0] /= 2;
			pm.pos = {this->_pos[0] + this->_size[0] / 2 - pm.size[0] / 2, pm.pos[1] + 100};

			this->AddElement("BTN_Cancel", std::make_unique<Buttons::TexturedButton>(pm), 1);
		};

		
		
		{ // Worlds List + LST_Worlds
			auto worldLst = LoadGamesDatas();

			std::vector<std::unique_ptr<AElement>> lst;
			lst.reserve(worldLst.size());

			for (auto world : worldLst)
			{
				Elements::WorldDataDisplayer::Vox_WorldDataDisplayer_Constructor st{
					{100, 100},
					{100, 125},
					world
				};
				lst.push_back(std::make_unique<Elements::WorldDataDisplayer>(st));
			}

			ScrollableList::Vox_ScrollableList_Constructor pm{};
			pm.pos = {this->_pos[0] +  (this->_size[0] / 3), this->_pos[1] + this->_size[1] / 5};
			pm.size = {this->_size[0] / 3, 501};
			pm.content = std::move(lst);


			this->AddElement("LST_Worlds", std::make_unique<ScrollableList>(pm), 1);
		}
		
		this->GetElement<Buttons::TexturedButton>("BTN_Cancel")
			->onClickCallbacks.AddCallBack(
				[](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;
					auto &im = Front::Interfaces::InterfacesManager::GetInstance();
					im.DisableInterface("World");
					im.EnableInterface("Main");
				});

		this->GetElement<Buttons::TexturedButton>("BTN_Join")->ChangeEnableState(false);
	}

	I_MenuWorld::~I_MenuWorld() {}

	void I_MenuWorld::SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->FindElement("IMG_BG")->elem->SetPos(this->_pos);
		// this->FindElement("TXT_Test")->elem->SetPos({100, 100});
		// this->FindElement("BTN_Test1")->elem->SetPos({400, 200});
		// this->FindElement("BTN_Test")->elem->SetPos({600, 200});
	}

	void I_MenuWorld::SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		_size[0] = MGL::Utils::findNextMultiple(newSize[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(newSize[1], 16.f);
		this->FindElement("IMG_BG")->elem->SetSize(this->_size);
		// this->FindElement("TXT_Test")->elem->SetSize({this->_size[0] - 200, this->_size[1] - 200});
	}

	void I_MenuWorld::Render()
	{
		for (auto &elem : this->_content)
			if (elem.elem)
				elem.elem->Draw();
		// std::cout << "render interface" << std::endl;
	}
} // namespace Vox::Game::Scenes::Menu::Interfaces