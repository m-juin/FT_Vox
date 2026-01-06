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
					Vector2Float pos;
					Vector2Float size;
					Saves::WorldData sd;
			};

			void SetPos(const Vector2Float newPos) override;
			void SetSize(const Vector2Float newSize) override;

			WorldDataDisplayer() = delete;
			WorldDataDisplayer(const Vox_WorldDataDisplayer_Constructor &st);
			~WorldDataDisplayer() {};

			void Select();
			void UnSelect();
			virtual bool IsHover(const Vector2Float &mousePos) override;
			virtual void OnClick(const int &button, const int &action) override;

			Saves::WorldData GetWorld() {return this->_linkedWorld;};

		private:
			Saves::WorldData _linkedWorld;
			/* private */
	};
	

} // namespace Vox::Game::Scenes::Menu::Interfaces::Elements

#endif // __WORLDDATADISPLAYER_HPP__