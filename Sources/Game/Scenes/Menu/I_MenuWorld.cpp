#include "Game/Scenes/Menu/I_MenuWorld.hpp"

#include "Front/Interfaces/Elements/Buttons/ColoredButton.hpp"
#include "Front/Interfaces/Elements/Buttons/TexturedButton.hpp"
#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/ScrollableList.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

#include "Front/Interfaces/InterfacesManager.hpp"

#include "MathGraphicalLib/Utils.hpp"

#include <GLFW/glfw3.h>

#include "Front/Window.hpp"

#include <vector>

#include "Game/Scenes/Menu/SavesData.hpp"

#include "Game/Scenes/Menu/InterfacesElements/WorldDataDisplayer.hpp"
#include "Game/Scenes/Menu/InterfacesElements/WorldDeletionCheck.hpp"

#include "Game/GameManager.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	using namespace Front::Interfaces::Elements;

	std::vector<Saves::WorldData> LoadGamesDatas()
	{
		std::vector<Saves::WorldData> lst;

		const std::string path = Saves::WorldsFolder;
		if (std::filesystem::exists(path) == false)
			return {};

		for (const auto &entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory() == false)
				continue;

			std::filesystem::path saveFile(entry.path() / "SaveData.json");
			if (std::filesystem::is_regular_file(saveFile) == false)
				continue;
			auto wd = Saves::LoadWorldData(saveFile.string());

			if (wd.folderPath == "")
			{
				std::cout << saveFile << std::endl;
				continue;
			}
			lst.push_back(wd);
		}
		return lst;
	}

	I_MenuWorld::I_MenuWorld(Vox::Front::Interfaces::Elements::Vector2 pos,
							 Vox::Front::Interfaces::Elements::Vector2 size)
		: AInterface(pos, size, false)
	{
		this->_inputMask |= Game::Utils::Datas::InputMask::Mouse | Game::Utils::Datas::InputMask::KeyBoard;
		_size[0] = MGL::Utils::findNextMultiple(size[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(size[1], 16.f);
		// std::cout << size << "\n" << std::endl;
		this->AddElement("IMG_BG",
						 std::make_unique<Image>("Menu_Main", "Dirt", this->_pos, this->_size,
												 Color(0.3f, 0.3f, 0.3f, 1.0f),
												 Vox::Front::Interfaces::Elements::Vector2(16, 16)),
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

			auto btn = std::make_unique<Buttons::TexturedButton>(pm);

			btn->onClickCallbacks.AddCallBack(
				[this](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;
					Game::GameManager::GetInstance().SetSaveData(this->_selectedWorld->GetWorld());
					Game::GameManager::GetInstance().GetSceneManager().LoadScene("World");
				});

			this->AddElement("BTN_Join", std::move(btn), 1);

			pm.content = "Create new World";
			pm.pos[0] = this->_pos[0] + this->_size[0] / 2 + 100;

			this->AddElement("BTN_Create", std::make_unique<Buttons::TexturedButton>(pm), 1);

			pm.content = "Cancel";
			pm.pos = {this->_pos[0] + this->_size[0] / 2 - pm.size[0] - 100, pm.pos[1] + 100};

			this->AddElement("BTN_Cancel", std::make_unique<Buttons::TexturedButton>(pm), 1);

			pm.content = "Delete";
			pm.pos[0] = this->_pos[0] + this->_size[0] / 2 + 100;

			this->AddElement("BTN_Delete", std::make_unique<Buttons::TexturedButton>(pm), 1);
		};

		{ // Worlds List + LST_Worlds
			auto worldLst = LoadGamesDatas();

			std::vector<std::unique_ptr<AElement>> lst;
			lst.reserve(worldLst.size());

			for (auto world : worldLst)
			{
				Elements::WorldDataDisplayer::Vox_WorldDataDisplayer_Constructor st{{100, 100}, {100, 125}, world};
				auto elem = std::make_unique<Elements::WorldDataDisplayer>(st);

				Elements::WorldDataDisplayer *elem2 = elem.get();
				elem->onClickCallbacks.AddCallBack(
					[this, elem2](const int &button, const int &action)
					{
						if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
							return;

						this->SetSelectedWorld(elem2);
					});

				lst.push_back(std::move(elem));
			}

			ScrollableList::Vox_ScrollableList_Constructor pm{};
			pm.pos = {this->_pos[0] + (this->_size[0] / 3), this->_pos[1] + this->_size[1] / 5};
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

		this->GetElement<Buttons::TexturedButton>("BTN_Create")
			->onClickCallbacks.AddCallBack(
				[](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;
					auto &im = Front::Interfaces::InterfacesManager::GetInstance();
					im.DisableInterface("World");
					im.EnableInterface("Create");
				});

		this->GetElement<Buttons::TexturedButton>("BTN_Join")->ChangeEnableState(false);

		auto btn = this->GetElement<Buttons::TexturedButton>("BTN_Delete");
		btn->ChangeEnableState(false);
		btn->onClickCallbacks.AddCallBack(
			[this](const int &button, const int &action)
			{
				if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
					return;
				this->WorldDeletionCheck();
			});
	}

	I_MenuWorld::~I_MenuWorld() {}

	void I_MenuWorld::SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->FindElement("IMG_BG")->elem->SetPos(this->_pos);
	}

	void I_MenuWorld::SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		_size[0] = MGL::Utils::findNextMultiple(newSize[0], 16.f);
		_size[1] = MGL::Utils::findNextMultiple(newSize[1], 16.f);
		this->FindElement("IMG_BG")->elem->SetSize(this->_size);
	}

	void I_MenuWorld::Render()
	{
		for (auto &elem : this->_content)
		{
			if (elem.elem && elem.isVisible)
			{
				elem.elem->Draw();
			}
		}
	}

	void I_MenuWorld::SetSelectedWorld(Elements::WorldDataDisplayer *elem)
	{
		if (elem == this->_selectedWorld)
			return;
		if (this->_selectedWorld != nullptr && elem != nullptr)
			this->_selectedWorld->UnSelect();
		if (elem != nullptr)
			elem->Select();
		this->_selectedWorld = elem;
		auto button = this->GetElement<Buttons::AButton>("BTN_Join");
		button->ChangeEnableState(elem != nullptr);
		this->GetElement<Buttons::AButton>("BTN_Delete")->ChangeEnableState(elem != nullptr);
	}

	void I_MenuWorld::WorldDeletionCheck()
	{
		if (this->_selectedWorld == nullptr)
			throw std::runtime_error("This should never happen");
		Elements::WorldDeletionCheck::Vox_WorldDeletionCheck_Constructor pm{
			{this->_pos[0] + this->_size[0] / 2 - 400, this->_pos[1] + this->_size[1] / 2 - 250},
			{800, 500},
			this->_selectedWorld->GetWorld()};

		auto elem = std::make_unique<Elements::WorldDeletionCheck>(pm);
		elem->GetElement<Buttons::TexturedButton>("BTN_Cancel")
			->onClickCallbacks.AddCallBack(
				[this](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;
					this->RemoveElement("WDC");
				});
		elem->GetElement<Buttons::TexturedButton>("BTN_Confirm")
			->onClickCallbacks.AddCallBack(
				[this](const int &button, const int &action)
				{
					if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_RELEASE)
						return;
					this->RemoveElement("WDC");
					if (this->_selectedWorld == nullptr)
						throw std::runtime_error("This should never happen!");
					auto lst = this->GetElement<ScrollableList>("LST_Worlds");
					lst->RemoveElement(lst->GetElementIndex(this->_selectedWorld));
					Saves::DeleteWorld(this->_selectedWorld->GetWorld());
					this->SetSelectedWorld(nullptr);
				});
		this->AddElement("WDC", std::move(elem), 2);
	}
} // namespace Vox::Game::Scenes::Menu::Interfaces