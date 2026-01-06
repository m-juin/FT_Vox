#ifndef __INPUTFIELD_HPP__
#define __INPUTFIELD_HPP__

#include "./Bases/AContainer.hpp"
#include "./Bases/AFocusable.hpp"

#include <cstdint>

namespace Vox::Front::Interfaces::Elements
{
	class InputField : public virtual Bases::AContainer, public virtual Bases::AFocusable
	{

		public:
			enum class E_InputMode
			{
				Alphabetic = 0,
				Numeric = 1,
				AlphaNumeric = 2,
				All = 3,
			};
			struct Constructor
			{
					Vector2Float pos;
					Vector2Float size;

					std::string BGAtlas;
					std::string BGAtlasKey;

					E_InputMode inputMode;
					std::string defaultValue;
					float inputScale;
					Constructor() : defaultValue (""), inputScale(0.5f) {};
			};

			InputField(const Constructor &st);
			~InputField() {};
			void SetPos(const Vector2Float newPos) override;
			void SetSize(const Vector2Float newSize) override;

			void Draw() override;

			inline bool IsHover(const Vector2Float &mousePos) override
			{
				return AClickable::IsHover(mousePos);
			};

			const std::string &GetValue();

			void OnClick(const int &button, const int &action) override;

			void HandleCharInput(const unsigned int &code);
			void HandleKeyInput(const unsigned int &code, const int &action);

			void UpdateIndicatorPos();

			void SetValue(const std::string &newVal);

			Vox::Utils::CallBacksManager<const std::string &> onTextChange;

		private:
			float _inputScale;
			uint8_t drawCount;
			size_t curPos;
			E_InputMode _inputMode;
			void ResetVertex() override {};

			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __INPUTFIELD_HPP__