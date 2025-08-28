#ifndef __WORLDDELETIONCHECK_HPP__
#define __WORLDDELETIONCHECK_HPP__

#include "Front/Interfaces/Elements/Bases/AContainer.hpp"

#include "Game/Scenes/Menu/SavesData.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces::Elements
{
	using namespace Vox::Front::Interfaces::Elements;
    class WorldDeletionCheck : public Bases::AContainer
    {
        public:
			struct Vox_WorldDeletionCheck_Constructor
			{
                Vector2 pos;
                Vector2 size;
                Saves::WorldData sd;
			};

            WorldDeletionCheck() = delete;
            WorldDeletionCheck(const Vox_WorldDeletionCheck_Constructor &st);

            void OnEnable() override {};
			void OnDisable() override {};
			void OnHover() override {};
			void OnHoverLeave() override {};

			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;

            ~WorldDeletionCheck() {};
    
        private:
            Saves::WorldData _linkedWorld;
            /* private */
    
    };
    
    
}

#endif // __WORLDDELETIONCHECK_HPP__