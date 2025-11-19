#ifndef __ASCENE_HPP__
#define __ASCENE_HPP__

#include <memory>
#include <string>

#include "Game/Utils/Datas/E_InputAction.hpp"

namespace Vox::Front::Scenes
{
	class AScene
	{
		public:
            virtual void InitSceneData() = 0;
			virtual ~AScene() = default;
			virtual void Load() = 0;
			virtual void Unload() = 0;
			virtual void Render() = 0;
			virtual const std::string &GetName() const = 0;
			virtual bool HandleInputAction(const Game::Utils::Datas::E_InputAction &action) = 0;

        protected:



	};
} // namespace Vox::Front::Scenes

#endif // __ASCENE_HPP__