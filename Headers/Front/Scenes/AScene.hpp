#ifndef __ASCENE_HPP__
#define __ASCENE_HPP__

#include <string>
#include <memory>

namespace Vox::Front::Scenes
{
    class AScene
    {
    public:
        virtual ~AScene() = default;
        virtual void Load() = 0;
        virtual void Unload() = 0;
        virtual void Draw() = 0;
        virtual const std::string& GetName() const = 0;
    };
} // namespace Vox::Front::Scenes

#endif // __ASCENE_HPP__