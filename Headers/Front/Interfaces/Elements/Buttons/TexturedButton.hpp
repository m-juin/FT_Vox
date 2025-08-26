#ifndef __TEXTUREDBUTTON_HPP__
#define __TEXTUREDBUTTON_HPP__

#include "AButton.hpp"

namespace Vox::Front::Interfaces::Elements::Buttons
{
    class TexturedButton : public AButton
    {
        public:
            struct Vox_TexturedButton_Constructor : public Vox_Button_Constructor
            {
                std::string atlas; 
                std::string atlasKey;

                std::string onHoverAtlas;
                std::string onHoverAtlasKey;

                
                
                Vox_TexturedButton_Constructor() : Vox_Button_Constructor(), atlas(""), atlasKey("") {};
            };
            
            TexturedButton() = delete;
            TexturedButton(const Vox_TexturedButton_Constructor &st);
            ~TexturedButton();
    
        private:
            /* private */
    
    };
} // namespace Vox::Front::Interfaces::Elements::Buttons


#endif // __TEXTUREDBUTTON_HPP__