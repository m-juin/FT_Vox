#ifndef __I_MENUWORLD_HPP__
#define __I_MENUWORLD_HPP__

#include "Front/Interfaces/AInterface.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces::Elements
{
	class WorldDataDisplayer;
}

namespace Vox::Game::Scenes::Menu::Interfaces
{
	class I_MenuWorld : public virtual Front::Interfaces::AInterface
	{
		public:
			I_MenuWorld(Vox::Front::Interfaces::Elements::Vector2Float pos = {0, 0}, Vox::Front::Interfaces::Elements::Vector2Float size = {0, 0});
			~I_MenuWorld();

			void SetPos(const Vox::Front::Interfaces::Elements::Vector2Float newPos) override;
			void SetSize(const Vox::Front::Interfaces::Elements::Vector2Float newSize) override;
			void Render() override;


			void SetSelectedWorld(Elements::WorldDataDisplayer *elem);
			Elements::WorldDataDisplayer *GetSelectedWorld() {return this->_selectedWorld;};

			void WorldDeletionCheck();

		private:
			Elements::WorldDataDisplayer *_selectedWorld = nullptr;
			/* private */
	};
} // namespace Vox::Game::Scenes::Menu::Interfaces

#endif // __MENUWORLD_HPP__