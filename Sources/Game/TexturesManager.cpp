#include "Front/Scenes/TexturesManager.hpp"

namespace Vox::Front::Scenes
{
	TexturesManager::TexturesManager(const std::string &T_Path)
	{
		this->CreateMap(T_Path);
	}

	TexturesManager::~TexturesManager() {}

	void TexturesManager::CreateMap(const std::string &T_Path)
	{
		this->_texturesMap["Menu_Main"] = new Front::Utils::TexturesAtlas(
			{
				{"Dirt", T_Path + "GUI/dirt.png"},
				{"Button", T_Path + "GUI/button.png"},
				{"Button_Hover", T_Path + "GUI/button_highlighted.png"},
				{"Button_Disabled", T_Path + "GUI/button_disabled.png"},
			},
			4);
		// this->_texturesMap["World_Block"] = new Front::Utils::TexturesAtlas({

		// });
		this->_fontImage = std::make_unique<Front::Rendering::Images::FontImage>(T_Path + "GUI/Fonts/Minecraft.ttf");
	}
} // namespace Vox::Front::Scenes
