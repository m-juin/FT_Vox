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
					Vector2 pos;
					Vector2 size;

					std::string BGAtlas;
					std::string BGAtlasKey;

					E_InputMode inputMode;
			};

			InputField(const Constructor &st);
			~InputField() {};
			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;

			void Draw() override;

			inline bool IsHover(const Vector2 &mousePos) override
			{
				return AClickable::IsHover(mousePos);
			};

			void OnClick(const int &button, const int &action) override;

			void HandleCharInput(const unsigned int &code);
			void HandleKeyInput(const unsigned int &code, const int &action);

			void UpdateIndicatorPos();

		private:
			uint8_t drawCount;
			size_t curPos;
			E_InputMode _inputMode;
			void ResetVertex() override {};
			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __INPUTFIELD_HPP__