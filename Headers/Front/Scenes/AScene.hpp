#ifndef __ASCENE_HPP__
#define __ASCENE_HPP__

#include <memory>
#include <string>

// #include "Front/Scenes/TexturesManager.hpp"

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

			// TexturesManager *GetTextureManager() {return this->_textureManager;};
			// TexturesManager *GetTextureManager() const {return this->_textureManager;};
        protected:
			// TexturesManager *_textureManager;

	};
} // namespace Vox::Front::Scenes

#endif // __ASCENE_HPP__