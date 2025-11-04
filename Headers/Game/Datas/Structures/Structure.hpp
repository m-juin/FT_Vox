#ifndef __STRUCTURE_HPP__
#define __STRUCTURE_HPP__

#include "Game/Datas/Blocks/BlocksType.hpp"

namespace Vox::Game::Datas::Structures
{
    class Structure
    {
        public:
            Structure() = delete;
            Structure(const std::string &path);
            ~Structure() {};
    
        private:
            /* private */
    
    };
    
} // namespace Vox::Game::Datas::Structures

#endif // __STRUCTURE_HPP__