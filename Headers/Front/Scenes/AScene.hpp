#ifndef __ASCENE_HPP__
#define __ASCENE_HPP__

#include <memory>
#include <string>

#include "Front/Scenes/ATexturesManager.hpp"

namespace Vox::Front::Scenes
{
	class ATextureManager;
	class AScene
	{
		public:
            virtual void InitSceneData() = 0;
			virtual ~AScene() = default;
			virtual void Load() = 0;
			virtual void Unload() = 0;
			virtual void Render() = 0;
			virtual const std::string &GetName() const = 0;

			ATexturesManager *GetTextureManager() {return this->_textureManager;};
			ATexturesManager *GetTextureManager() const {return this->_textureManager;};
        protected:
			ATexturesManager *_textureManager;

	};
} // namespace Vox::Front::Scenes

#endif // __ASCENE_HPP__