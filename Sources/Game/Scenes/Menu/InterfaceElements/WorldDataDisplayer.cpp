#include "Game/Scenes/Menu/InterfacesElements/WorldDataDisplayer.hpp"


#include "Front/Interfaces/Elements/Image.hpp"
#include "Front/Interfaces/Elements/Text.hpp"

namespace Vox::Game::Scenes::Menu::Interfaces::Elements
{
    WorldDataDisplayer::WorldDataDisplayer(const Vox_WorldDataDisplayer_Constructor &st) : Bases::AElement(st.pos, st.size), AClickable(), Bases::AContainer(), _linkedWorld(st.sd)
    {
        this->AddElement("IMG_BG", std::make_unique<Image>("Menu_Main", "Dirt", this->_pos, this->_size,
												 Color(0.7f, 0.7f, 0.7f, 1.0f), Vector2Float(2, 2)), 0);

        Vector2Float elemPos = {this->_pos[0] + 5, this->_pos[1] + 5};
        Vector2Float elemSize = {this->_size[1] - 10};

        this->AddElement("IMG_Preview", std::make_unique<Image>("", "", elemPos, elemSize,
												 Color(1.0f, 1.0f, 1.0f, 1.0f), Vector2Float(1, 1)), 1);

        {
            Text::Vox_Text_Constructor ElemSt{};
            ElemSt.content = st.sd.worldName;

            elemPos[0] += elemSize[0] + this->_size[1] - 10 - Text::GetTextSize(ElemSt.content, 0.7f)[0];

            ElemSt.color = {1.0, 1.0, 1.0, 1.0};
            ElemSt.pos = elemPos;
            ElemSt.size = (Text::GetTextSize(ElemSt.content, 0.7f)[0], 50);
            ElemSt.scale = 0.6f;
            this->AddElement("TXT_WorldName", std::make_unique<Text>(ElemSt), 1);
        }
    }

    void WorldDataDisplayer::SetPos(const Vector2Float newPos)
    {
        if (newPos == this->_pos) return ;
        this->_pos = newPos;

        this->GetElement<Image>("IMG_BG")->SetPos(this->_pos);

        Vector2Float elemPos = {this->_pos[0] + 5, this->_pos[1] + 5};
        Vector2Float elemSize = {this->_size[1] - 10};

        this->GetElement<Image>("IMG_Preview")->SetPos(elemPos);

        auto txt = this->GetElement<Text>("TXT_WorldName");

        elemPos[0] += this->_size[0] - 25 - txt->GetTextSize()[0];
        elemPos[1] += 30;
        txt->SetPos(elemPos);
    }
    
    void WorldDataDisplayer::SetSize(const Vector2Float newSize)
    {
        if (newSize == this->_size) return ;
        this->_size = newSize;

        this->GetElement<Image>("IMG_BG")->SetSize(this->_size);

        Vector2Float elemSize = {this->_size[1] - 10};

        this->GetElement<Image>("IMG_Preview")->SetSize(elemSize);
    }

	void WorldDataDisplayer::OnClick(const int &button, const int &action)
	{
		AClickable::OnClick(button, action);
	}

	bool WorldDataDisplayer::IsHover(const Vector2Float &mousePos)
	{
		return AClickable::IsHover(mousePos);
	}

    void WorldDataDisplayer::Select()
	{
		this->GetElement<Image>("IMG_BG")->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}
	
	void WorldDataDisplayer::UnSelect()
	{
		this->GetElement<Image>("IMG_BG")->SetColor({0.7f, 0.7f, 0.7f, 1.0f});
	}
}