#ifndef __ACLICKABLE_HPP__
#define __ACLICKABLE_HPP__

#include "./AElement.hpp"

#include "Utils/CallBacksManager.hpp"


namespace Vox::Front::Interfaces::Elements::Bases
{
	
	class AClickable : public virtual AElement
	{
		public:

			AClickable() : _enabled(true) {};
			virtual ~AClickable() {};
			virtual void OnEnable() = 0;
			virtual void OnDisable() = 0;
			virtual void OnHover() = 0;
			virtual void OnClick(const int &button, const int &action) = 0;
			virtual void OnHoverLeave() = 0;
			virtual bool IsHover(const Vector2 &mousePos, bool override = false)
			{
				if (this->_enabled == false) return false;
				if (override == false && (mousePos[0] >= this->_pos[0] && mousePos[0] <= this->_pos[0] + this->_size[0]) &&
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

			bool currentlyHovered = false;

			bool IsEnabled() {return this->_enabled;};

		protected:
			bool _enabled;


	};
} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __ACLICKABLE_HPP__