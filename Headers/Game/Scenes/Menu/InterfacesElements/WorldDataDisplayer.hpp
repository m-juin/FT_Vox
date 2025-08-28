#ifndef __WORLDDATADISPLAYER_HPP__
#define __WORLDDATADISPLAYER_HPP__

#include "Front/Interfaces/Elements/Bases/AContainer.hpp"
#include "Game/Scenes/Menu/SavesData.hpp"

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
					Saves::WorldData sd;
			};
			void OnEnable() override {};
			void OnDisable() override {};
			void OnHover() override {};
			void OnClick(const int &button, const int &action) override;
			void OnHoverLeave() override {};

			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;

			WorldDataDisplayer() = delete;
			WorldDataDisplayer(const Vox_WorldDataDisplayer_Constructor &st);

			Vox::Utils::CallBacksManager<const int &, const int &> onClickCallbacks;
			~WorldDataDisplayer() {};

			void Select();
			void UnSelect();

			Saves::WorldData GetWorld() {return this->_linkedWorld;};

		private:
			Saves::WorldData _linkedWorld;
			/* private */
	};
	
	

} // namespace Vox::Game::Scenes::Menu::Interfaces::Elements

#endif // __WORLDDATADISPLAYER_HPP__