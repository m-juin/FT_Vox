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
                Vector2Float pos;
                Vector2Float size;
                Saves::WorldData sd;
			};

            WorldDeletionCheck() = delete;
            WorldDeletionCheck(const Vox_WorldDeletionCheck_Constructor &st);

			void SetPos(const Vector2Float newPos) override;
			void SetSize(const Vector2Float newSize) override;

            ~WorldDeletionCheck() {};
    
        private:
            Saves::WorldData _linkedWorld;
            /* private */
    
    };
    
    
}

#endif // __WORLDDELETIONCHECK_HPP__