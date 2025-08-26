#ifndef __ACLICKABLE_HPP__
#define __ACLICKABLE_HPP__

#include "./AElement.hpp"

#include "Utils/CallBacksManager.hpp"


namespace Vox::Front::Interfaces::Elements::Bases
{
	namespace Constructors
	{
		
	} // namespace Constructors
	
	class AClickable : public AElement
	{
		public:

			AClickable(Vector2 pos = {0, 0}, Vector2 size = {100, 50}) : AElement(pos, size), _enabled(true) {};
			virtual ~AClickable() {};
			virtual void OnEnable() = 0;
			virtual void OnDisable() = 0;
			virtual void OnHover() = 0;
			virtual void OnClick(const int &button, const int &action) = 0;
			virtual void OnHoverLeave() = 0;
			virtual bool IsHover(const Vector2 &mousePos)
			{
				if ((mousePos[0] >= this->_pos[0] && mousePos[0] <= this->_pos[0] + this->_size[0]) &&
					(mousePos[1] >= this->_pos[1] && mousePos[1] <= this->_pos[1] + this->_size[1]))
				{
					if (currentlyHovered == true)
						return true;
					currentlyHovered = true;
					this->OnHover();
				}
				else
				{
					if (currentlyHovered == false)
						return false;
					currentlyHovered = false;
					this->OnHoverLeave();
				}
				return this->currentlyHovered;
			};

			virtual void ChangeEnableState(bool newState)
			{
				if (newState == this->_enabled) return;
				this->_enabled = newState;
				if (this->_enabled)
					OnEnable();
				else
					OnDisable();
			}

			virtual void SetPos(const Vector2 newPos) = 0;
			virtual void SetSize(const Vector2 newSize) = 0;
			bool currentlyHovered = false;

			bool IsEnabled() {return this->_enabled;};

		protected:
			bool _enabled;


	};
} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __ACLICKABLE_HPP__