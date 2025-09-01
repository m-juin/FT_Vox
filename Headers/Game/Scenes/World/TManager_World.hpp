#ifndef __TMANAGER_WORLD_HPP__
#define __TMANAGER_WORLD_HPP__

#include "Front/Scenes/ATexturesManager.hpp"

namespace Vox::Game::Scenes::World
{
    class TManager_World : public Front::Scenes::ATexturesManager
    {
        public:
            TManager_World();
            ~TManager_World();
            void CreateMap() override;
        private:
            /* private */
    
    };
} // namespace Vox::Game::Scenes::Menu


#endif // __TMANAGER_WORLD_HPP__