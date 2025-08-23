#ifndef __ACLICKABLE_HPP__
#define __ACLICKABLE_HPP__

#include "./AElement.hpp"

namespace Vox::Front::Interfaces::Elements::Bases
{
	class AClickable : public AElement
	{
		public:
			AClickable(Vector2 pos = {0, 0}, Vector2 size = {100, 50}) : AElement(pos, size) {};
			virtual ~AClickable() {};
			virtual void OnHover() = 0;
			virtual void OnClick() = 0;
			virtual void OnHoverLeave() = 0;
			virtual bool IsHover(const Vector2 &mousePos)
			{
				if ((mousePos[0] >= this->_pos[0] && mousePos[0] <= this->_pos[0] + this->_size[0]) &&
					(mousePos[1] >= this->_pos[1] && mousePos[1] <= this->_pos[1] + this->_size[1]))
				{
					if (_currentlyHovered == true)
						return true;
					_currentlyHovered = true;
					this->OnHover();
				}
				else
				{
					if (_currentlyHovered == false)
						return false;
					_currentlyHovered = false;
					this->OnHoverLeave();
				}
				return this->_currentlyHovered;
			};

			virtual void SetPos(const Vector2 newPos) = 0;
			virtual void SetSize(const Vector2 newSize) = 0;

		private:
			bool _currentlyHovered = false;
	};
} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __ACLICKABLE_HPP__