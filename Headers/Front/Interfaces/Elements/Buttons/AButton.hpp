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

		class AButton : public Bases::AContainer
		{
			public:
				struct Vox_Button_Constructor
				{
						Vector2 pos;
						Vector2 size;

						std::string content;
						Color textColor;
						// Color bgColor;
						//
						// Color hoverBGColor;
						Color hoverTXTColor;

						Vox_Button_Constructor()
							: pos({0, 0}), size({100, 50}), content("Button"), textColor({0.0, 0.0, 0.0, 1.0}),
							  hoverTXTColor(1.0, 1.0, 1.0, 1.0) {};
				};

				AButton() = delete;
				AButton(Vector2 pos_ = {0, 0}, Vector2 size_ = {100, 50}, const std::string &content_ = "Button",
						Color textColor_ = {0.0, 0.0, 0.0, 1.0});
				AButton(const Vox_Button_Constructor &st);
				virtual ~AButton();

				void OnClick(const int &button, const int &action) override;
				void OnHover() override; 
				void OnHoverLeave() override;
				bool IsHover(const Vector2 &mousePos) override;

				void SetPos(Vector2 newPos) override;
				void SetSize(Vector2 newSize) override;

				void ResetVertex() override;

			protected:
				Vox::Utils::CallBacksManager<> _onHoverCallbacks;
				Vox::Utils::CallBacksManager<> _onHoverLeaveCallbacks;
				Vox::Utils::CallBacksManager<const int &, const int &> _onClickCallbacks;
				// std::vector<ClickCallBack> _onClickCallbacks;

				/* private */
		};
	} // namespace Buttons
} // namespace Vox::Front::Interfaces::Elements

#endif // __BUTTON_HPP__