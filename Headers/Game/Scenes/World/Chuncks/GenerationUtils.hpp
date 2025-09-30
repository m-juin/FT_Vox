#ifndef __GENERATIONUTILS_HPP__
#define __GENERATIONUTILS_HPP__

#include <vector>
#include <algorithm>
#include "Game/Scenes/World/Chuncks/FacesData.hpp"
#include "Game/Utils/TexturesData.hpp"

namespace Vox::Game::Generation::Utils
{
    static const Game::Utils::Textures::TextureInfo &GetFaceTextureInfo(const std::vector<Game::Utils::Textures::TextureInfo> &lst,
																				const std::string &key,
																				Game::World::Chuncks::Faces face)
	{
		std::string_view faceKey = Game::World::Chuncks::ToString(face);
		std::string effectiveKey = key;

		if (faceKey != "")
		{
			effectiveKey += "_";
			effectiveKey += faceKey;
			auto it = std::find_if(lst.begin(), lst.end(),
									 [effectiveKey](const Game::Utils::Textures::TextureInfo &info) { return effectiveKey == info.key; });
			if (it != lst.end())
				return *it;
		}
		auto it = std::find_if(lst.begin(), lst.end(), [key](const Game::Utils::Textures::TextureInfo &info) { return key == info.key; });
		if (it != lst.end())
			return *it;
		throw std::runtime_error("Face Texture not found!");
	}
} // namespace Vox::Game::Generation::Utils


#endif // __GENERATIONUTILS_HPP__