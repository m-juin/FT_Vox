#ifndef __I_GENERATION_HPP__
#define __I_GENERATION_HPP__

#include "Front/Interfaces/AInterface.hpp"

#include "Utils/AUpdatable.hpp"
#include "./M_Minimap.hpp"

#include "MathGraphicalLib/Vectors/Defines.hpp"

using namespace Vox::Front::Interfaces;

namespace Vox::Game::Scenes::World::Interfaces
{
    using namespace MGL::Vectors::Types;
    class I_Generation : public virtual AInterface, public virtual Vox::Utils::AUpdatable
    {
        public:
            I_Generation(Vox::Front::Interfaces::Elements::Vector2Float pos, Vox::Front::Interfaces::Elements::Vector2Float size);
            I_Generation() = delete;
            ~I_Generation();
    
        private:
            M_Minimap _mMinimap;

            void UpdateMap(Vector2Int playerPos);
			void SetPos(const Vox::Front::Interfaces::Elements::Vector2Float newPos) override;
			void SetSize(const Vox::Front::Interfaces::Elements::Vector2Float newSize) override;
            void OnHover() override {};
            void OnHoverLeave() override {};
            void Render() override;
            /* private */
    
    };
}

#endif // __I_GENERATION_HPP__