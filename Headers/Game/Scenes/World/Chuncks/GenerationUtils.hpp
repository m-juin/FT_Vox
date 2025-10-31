#ifndef __GENERATIONUTILS_HPP__
#define __GENERATIONUTILS_HPP__

#include <vector>
#include <algorithm>
#include "Game/Scenes/World/Chuncks/FacesData.hpp"
#include "Game/Datas/TexturesData.hpp"

namespace Vox::Game::Generation::Utils
{
    static const Game::Datas::Textures::TextureInfo &GetFaceTextureInfo(const std::vector<Game::Datas::Textures::TextureInfo> &lst,
																				const Game::Datas::Blocks::BlockType &key,
																				Game::World::Chuncks::Faces face)
	{
		std::string_view faceKey = Game::World::Chuncks::ToString(face);


		std::string defaultKey(Game::Datas::Blocks::EnumToString.at(key));
	
		std::string effectiveKey(defaultKey);

		if (faceKey != "")
		{
			effectiveKey += "_";
			effectiveKey += faceKey;
			auto it = std::find_if(lst.begin(), lst.end(),
									 [effectiveKey](const Game::Datas::Textures::TextureInfo &info) { return effectiveKey == info.key; });
			if (it != lst.end())
				return *it;
		}
		auto it = std::find_if(lst.begin(), lst.end(), [defaultKey](const Game::Datas::Textures::TextureInfo &info) { return defaultKey == info.key; });
		if (it != lst.end())
			return *it;
		throw std::runtime_error("Face Texture not found!");
	}
} // namespace Vox::Game::Generation::Utils


#endif // __GENERATIONUTILS_HPP__