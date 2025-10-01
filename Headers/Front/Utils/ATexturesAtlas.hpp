#ifndef __ATEXTURESATLAS_HPP__
#define __ATEXTURESATLAS_HPP__

#include "Game/Utils/TexturesData.hpp"


#include "Front/Rendering/Images/VulkanImage.hpp"

#include <memory>
#include <vector>

#include <cstring>

namespace Vox::Front::Utils
{
	using TextureInfo = Game::Utils::Textures::TextureInfo;
	using namespace Vox::Front::Rendering::Images;

	class ATexturesAtlas
	{
		protected:
			static constexpr uint32_t MAX_TEXTURE_SIZE = 8192;
			struct TexturesDatas
			{
					const std::string key;
					const std::string path;

					TexturesDatas() = delete;
					TexturesDatas(const std::string &key_, const std::string &path_) : key(key_), path(path_) {};
			};
			uint32_t _atlasWidth;
			uint32_t _atlasHeight;

			std::vector<TextureInfo> _textureInfos;
			size_t _textureWidth;
			size_t _textureHeight;
			size_t _textureChannels;
			static uint32_t nextPowerOfTwo(uint32_t value);
			unsigned char *LoadSingleTexture(const std::string &path, uint8_t expectedChannel);


		public:
			struct uvData
			{
					MGL::Vectors::Vector2<float> uvMin;
					MGL::Vectors::Vector2<float> uvMax;
					MGL::Vectors::Vector2<uint16_t> atlasSize;
					int textureID;
			};
			const std::vector<TextureInfo> &GetTextureInfo() const
			{
				return this->_textureInfos;
			}
			const TextureInfo &GetTextureInfo(size_t index) const;
			const TextureInfo &GetTextureInfo(const std::string &key) const;
			std::unique_ptr<VulkanImage> _mainAtlas;
			ATexturesAtlas() = delete;
			ATexturesAtlas(const uint32_t texturesChannels = 4);
			virtual ~ATexturesAtlas() {};
	};

} // namespace Vox::Front::Utils

#endif // __ATEXTURESATLAS_HPP__