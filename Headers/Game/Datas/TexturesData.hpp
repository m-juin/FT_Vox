#ifndef __TEXTURESDATA_HPP__
#define __TEXTURESDATA_HPP__

#include <iostream>

namespace Vox::Game::Datas::Textures
{
	constexpr const char *TPacks_Path = "Assets/TexturesPacks/";

	struct TextureInfo
	{
			const std::string key;
			float uOffset;
			float vOffset;
			float uSize;
			float vSize;
	};
} // namespace Vox::Game::Utils::Textures

#endif // __TEXTURESDATA_HPP__