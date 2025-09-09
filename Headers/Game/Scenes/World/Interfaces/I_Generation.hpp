#ifndef __I_GENERATION_HPP__
#define __I_GENERATION_HPP__

#include "Front/Interfaces/AInterface.hpp"

#include "Utils/AUpdatable.hpp"

using namespace Vox::Front::Interfaces;

namespace Vox::Game::Scenes::World::Interfaces
{
    class I_Generation : public virtual AInterface, public virtual Vox::Utils::AUpdatable
    {
        public:
            I_Generation(Vox::Front::Interfaces::Elements::Vector2 pos, Vox::Front::Interfaces::Elements::Vector2 size);
            I_Generation() = delete;
            ~I_Generation();
    
        private:

			void SetPos(const Vox::Front::Interfaces::Elements::Vector2 newPos) override;
			void SetSize(const Vox::Front::Interfaces::Elements::Vector2 newSize) override;
            void OnHover() override {};
            void OnHoverLeave() override {};
            void Render() override;

            void UpdateSeed();
            /* private */
    
    };
}

#endif // __I_GENERATION_HPP__