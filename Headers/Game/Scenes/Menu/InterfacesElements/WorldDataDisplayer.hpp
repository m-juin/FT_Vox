#ifndef __WORLDDATADISPLAYER_HPP__
#define __WORLDDATADISPLAYER_HPP__

#include "Front/Interfaces/Elements/Bases/AContainer.hpp"
#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

namespace Vox::Game::Scenes::Menu::Saves
{
	struct WorldData;
};

namespace Vox::Game::Scenes::Menu::Interfaces::Elements
{
	using namespace Vox::Front::Interfaces::Elements;

	class WorldDataDisplayer : public Bases::AContainer
	{

		public:
			struct Vox_WorldDataDisplayer_Constructor
			{
					Vector2 pos;
					Vector2 size;
					const Saves::WorldData &sd;
			};
			void OnEnable() override {};
			void OnDisable() override {};
			void OnHover() override {};
			void OnClick(const int &button, const int &action) override {(void)button; (void)action;};
			void OnHoverLeave() override {};

			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;

			WorldDataDisplayer() = delete;
			WorldDataDisplayer(const Vox_WorldDataDisplayer_Constructor &st);
			~WorldDataDisplayer() {};

		private:
			/* private */
	};

} // namespace Vox::Game::Scenes::Menu::Interfaces::Elements

#endif // __WORLDDATADISPLAYER_HPP__