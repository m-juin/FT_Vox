#ifndef __I_F3_HPP__
#define __I_F3_HPP__

#include "Front/Interfaces/AInterface.hpp"

#include "Utils/AUpdatable.hpp"

using namespace Vox::Front::Interfaces;

namespace Vox::Game::Scenes::World::Interfaces
{
    class I_F3 : public virtual AInterface, public virtual Vox::Utils::AUpdatable
    {
        public:
            I_F3(Vox::Front::Interfaces::Elements::Vector2Float pos, Vox::Front::Interfaces::Elements::Vector2Float size);
            I_F3() = delete;
            ~I_F3();
    
        private:

			void SetPos(const Vox::Front::Interfaces::Elements::Vector2Float newPos) override;
			void SetSize(const Vox::Front::Interfaces::Elements::Vector2Float newSize) override;
            void OnHover() override {};
            void OnHoverLeave() override {};
            void Render() override;

            void UpdatePlayerPos();
            void UpdateGenerationCount();
            void UpdateBiomeInfos();
            /* private */
    
    };
}

#endif // __I_F3_HPP__