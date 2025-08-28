#ifndef __I_MENUWORLD_HPP__
#define __I_MENUWORLD_HPP__

#include "Front/Interfaces/AInterface.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	class I_MenuWorld : public virtual Front::Interfaces::AInterface
	{
		public:
			I_MenuWorld(Vox::Front::Interfaces::Elements::Vector2 pos = {0, 0}, Vox::Front::Interfaces::Elements::Vector2 size = {0, 0});
			~I_MenuWorld();

			void SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos) override;
			void SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize) override;
			void Render() override;

			void OnHover() override {};
			void OnHoverLeave() override {};

		private:
			/* private */
	};
} // namespace Vox::Game::Scenes::Menu::Interfaces

#endif // __MENUWORLD_HPP__