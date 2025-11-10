#ifndef __STRUCTURE_HPP__
#define __STRUCTURE_HPP__

#include "Game/Datas/Blocks/BlocksType.hpp"

#include <vector>
#include "MathGraphicalLib/Vectors/Vector3.hpp"

namespace Vox::Game::Datas::Structures
{
    class Structure
    {
        public:
            Structure() = delete;
            Structure(const std::string &path);
            ~Structure() {};

            const std::vector<Blocks::BlockType> &GetContent() {return this->_content;};
            MGL::Vectors::Vector3<size_t> _structureSize;
            MGL::Vectors::Vector3<size_t> _anchorPoint;
            size_t GetLocalIndex(MGL::Vectors::Vector3<size_t> pos) const;
            size_t GetLocalIndex(MGL::Vectors::Vector3<int> pos) const;
            MGL::Vectors::Vector3<size_t> FromIndexToVector(size_t index) const;
        private:
            void BuildContent(std::vector<std::string> fileData, const std::map<std::string, Game::Datas::Blocks::BlockType> &mapping);
            std::vector<Blocks::BlockType> _content;
            /* private */
    
    };
    
} // namespace Vox::Game::Datas::Structures

#endif // __STRUCTURE_HPP__