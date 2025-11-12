#ifndef __BLOCKDATA_HPP__
#define __BLOCKDATA_HPP__

#include "MathGraphicalLib/Vectors/Vector3.hpp"

#include "Game/Datas/Biomes.hpp"
#include "Game/Datas/Blocks/BlocksType.hpp"

#include "Game/Scenes/World/Chuncks/FacesData.hpp"

namespace Vox::Game::Datas::Blocks
{
    struct BlockFaceData
    {
        bool isVisible;
        bool isTransparent;
        Vox::Game::World::Chuncks::Faces faceDirection;

        BlockFaceData(Vox::Game::World::Chuncks::Faces direction_) : isVisible(false), isTransparent(false), faceDirection(direction_) {};
    };
    
    struct BlockData
    {
        BlockType type;
        std::array<BlockFaceData, 6> _faces = {
            BlockFaceData(Vox::Game::World::Chuncks::Faces::TOP),
            BlockFaceData(Vox::Game::World::Chuncks::Faces::BOT),
            BlockFaceData(Vox::Game::World::Chuncks::Faces::LEFT),
            BlockFaceData(Vox::Game::World::Chuncks::Faces::RIGHT),
            BlockFaceData(Vox::Game::World::Chuncks::Faces::FRONT),
            BlockFaceData(Vox::Game::World::Chuncks::Faces::BACK),
        };

        inline void UpdateFacesVisibility(std::array<bool, 6> newData)
        {
            for (uint8_t i = 0; i < 6; i++)
            {
                this->_faces[i].isVisible = newData[i];
            }
            
        }
        inline void UpdateFaceVisibility(bool newVisibility, Vox::Game::World::Chuncks::Faces faceDir)
        {
            this->_faces[static_cast<int>(faceDir)].isVisible = newVisibility;
        }

        inline void UpdateFacesTransparency(std::array<bool, 6> newData)
        {
            for (uint8_t i = 0; i < 6; i++)
            {
                this->_faces[i].isTransparent = newData[i];
            }
            
        }

        inline const std::array<BlockFaceData, 6> &GetFacesData() {return this->_faces;}
    };

} // namespace Vox::Game::Datas::Blocks


#endif // __BLOCKDATA_HPP__