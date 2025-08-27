#ifndef __I_MENUWORLD_HPP__
#define __I_MENUWORLD_HPP__

#include "Front/Interfaces/AInterface.hpp"

#include "MathGraphicalLib/Vectors/Vector2.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces
{
	using Vector2 = MGL::Vectors::Vector2<size_t>;
	class I_MenuWorld : public virtual Front::Interfaces::AInterface
	{
		public:
			I_MenuWorld(Vector2 pos = {0, 0}, Vector2 size = {0, 0});
			~I_MenuWorld();

			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;
			void Render() override;

			void OnHover() override {};
			void OnHoverLeave() override {};

		private:
			/* private */
	};
} // namespace Vox::Game::Scenes::Menu::Interfaces

#endif // __MENUWORLD_HPP__