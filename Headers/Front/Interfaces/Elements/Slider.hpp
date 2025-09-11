#ifndef __SLIDER_HPP__
#define __SLIDER_HPP__

#include "./Bases/AContainer.hpp"
#include "./Bases/AFocusable.hpp"

#include "Utils/CallBacksManager.hpp"

namespace Vox::Front::Interfaces::Elements
{
	class Slider : public virtual Bases::AContainer, public virtual Bases::AFocusable
	{
		public:
			struct Constructor
			{
					Vector2 pos;
					Vector2 size;

					Color BGColor;
					Color HandleColor;
					Color HandleSlidingColor;

					float defaultValue;
					float minVal;
					float maxVal;
					float step;

					Constructor()
						: pos({0, 0}), size({100, 20}), BGColor({1.0, 1.0, 1.0, 1.0}),
						  HandleColor({0.5, 0.5, 0.5, 1.0}), HandleSlidingColor({0.0, 1.0, 0.0, 1.0}),
						  defaultValue(0.0f), minVal(0.0f), maxVal(1.0f), step(0.01f) {};
			};

			Slider() = delete;
			Slider(const Constructor &st);
			void SetMinVal(const float &newMin);
			void SetMaxVal(const float &newMax);
			void SetStep(const float &newStep);
			void SetValue(const float &newVal);

			void SetPos(Vector2 newPos) override;
			void SetSize(Vector2 newSize) override;

			const float &GetMinVal()
			{
				return this->_range.first;
			};
			const float &GetMaxVal()
			{
				return this->_range.second;
			};
			const float &GetStep()
			{
				return this->_step;
			};
			const float &GetValue()
			{
				return this->_value;
			};
			~Slider() {};

			void OnClick(const int &button, const int &action) override;
			bool IsHover(const Vector2 &mousePos) override;

			void HandleMovement(float xPos);

			Vox::Utils::CallBacksManager<const float &, const float &> onValueChange;

		private:
			void UpdateVals();
			float GetHandlePos();
			float _value;
			float _step;
			std::pair<float, float> _range;
			Color _handleColor;
			Color _FocusedHandleColor;
			float prevX;
			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements

#endif //__SLIDER_HPP__