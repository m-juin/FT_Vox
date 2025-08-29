#ifndef __COLOREDBUTTON_HPP__
#define __COLOREDBUTTON_HPP__

#include "AButton.hpp"

namespace Vox::Front::Interfaces::Elements::Buttons
{
	class ColoredButton : public AButton
	{
		public:
			struct Vox_ColorButton_Constructor : public AButton::Vox_Button_Constructor
			{
					Color bgColor;
					Color hoverBGColor;
					Color disabledBGColor;

					Vox_ColorButton_Constructor()
						: AButton::Vox_Button_Constructor(), bgColor({0.8, 0.8, 0.8, 1.0}),
						  hoverBGColor({0.5, 0.5, 0.5, 1.0}), disabledBGColor({0.2, 0.2, 0.2, 1.0}) {};
			};
			ColoredButton() = delete;
			ColoredButton(const Vox_ColorButton_Constructor &st);
			~ColoredButton() {};

		private:
			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements::Buttons

#endif // __COLOREDBUTTON_HPP__