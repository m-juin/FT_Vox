#ifndef __SC_MENU_HPP__
#define __SC_MENU_HPP__

#include "Front/Scenes/AScene.hpp"
#include "Front/Scenes/ScenesManager.hpp"
#include <memory>
#include <string>

#include <iostream>

namespace Vox::Game::Scenes::Menu
{
	class Sc_Menu : public Front::Scenes::AScene
	{
		public:
			static constexpr const char *getName()
			{
				return "Menu";
			}

			Sc_Menu() : AScene() {}
			~Sc_Menu() override = default;

			void Load() override;
			void Unload() override;
			void Render() override;
			void InitSceneData() override;
			bool HandleInputAction(const Game::Utils::Datas::E_InputAction &action) override {(void)action; return true;};

			const std::string &GetName() const override
			{
				static const std::string name = getName();
				return name;
			}
	};

} // namespace Vox::Game::Scenes::Menu

template <> void Vox::Front::Scenes::RegisterSceneFactory<Vox::Game::Scenes::Menu::Sc_Menu>();

#endif // __SC_MENU_HPP__