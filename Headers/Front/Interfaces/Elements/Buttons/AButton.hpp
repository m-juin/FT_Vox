#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include "../Bases/AContainer.hpp"

#include "../Utils/Defines.hpp"

namespace Vox::Front::Interfaces::Elements
{
	class Text;
	class Image;

	namespace Buttons
	{

		class AButton : public virtual Bases::AContainer
		{
			public:
				struct Vox_Button_Constructor
				{
						Vector2 pos;
						Vector2 size;

						std::string content;
						Color textColor;

						float textScale;
						// Color bgColor;
						//
						// Color hoverBGColor;
						Color hoverTXTColor;
						Color disabledTXTColor;

						Vox_Button_Constructor()
							: pos({0, 0}), size({100, 50}), content("Button"), textColor({0.0, 0.0, 0.0, 1.0}),
							  textScale(0.5f), hoverTXTColor({1.0, 1.0, 1.0, 1.0}),
							  disabledTXTColor({0.7, 0.7, 0.7, 1.0}) {};
				};

				AButton() = delete;
				AButton(const std::string &content_ = "Button", Color textColor_ = {0.0, 0.0, 0.0, 1.0});
				AButton(const Vox_Button_Constructor &st);
				virtual ~AButton();
				inline virtual bool IsHover(const Vector2 &mousePos) override
				{
					return AClickable::IsHover(mousePos); // <- appel direct
				}

				inline virtual void OnClick(const int &button, const int &action) override
				{
					AClickable::OnClick(button, action);
				}
				virtual void SetPos(Vector2 newPos) override;
				virtual void SetSize(Vector2 newSize) override;

				void ResetVertex() override;

			protected:
				// std::vector<ClickCallBack> _onClickCallbacks;

				/* private */
		};
	} // namespace Buttons
} // namespace Vox::Front::Interfaces::Elements

#endif // __BUTTON_HPP__