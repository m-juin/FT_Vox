#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "MathGraphicalLib/Matrix/Matrix4.hpp"

namespace Vox::Game::Scenes::World::Player
{
    struct CameraInfo
    {
        MGL::Matrix::Matrix4 projection;
        MGL::Matrix::Matrix4 view;
    };
    
} // namespace Vox::Game::Scenes::World::Player


#endif // __CAMERA_HPP__