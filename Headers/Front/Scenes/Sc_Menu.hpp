#ifndef __SC_MENU_HPP__
#define __SC_MENU_HPP__

#include "AScene.hpp"
#include "ScenesManager.hpp"
#include <string>
#include <memory>

namespace Vox::Front::Scenes
{
    class Sc_Menu : public AScene
    {
    public:
        static constexpr const char* getName()
        {
            return "menu";
        }
        
        Sc_Menu() : AScene() {}
        ~Sc_Menu() override = default;
        
        void Load() override;
        void Unload() override;
        void Draw() override;
        
        const std::string& GetName() const override
        {
            static const std::string name = getName();
            return name;
        }
    };
    
    // Enregistrement de la scène dans le registre global
    template <>
    void RegisterSceneFactory<Sc_Menu>();
} // namespace Vox::Front::Scenes

#endif // __SC_MENU_HPP__