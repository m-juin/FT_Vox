#ifndef __ACLICKABLE_HPP__
#define __ACLICKABLE_HPP__

#include "./AElement.hpp"

#include "Utils/CallBacksManager.hpp"
#include "MathGraphicalLib/Vectors/Defines.hpp"

namespace Vox::Front::Interfaces
{
	class InterfacesManager;
}

namespace Vox::Front::Interfaces::Elements::Bases
{

	class AClickable : public virtual AElement
	{
		public:
			friend class Vox::Front::Interfaces::InterfacesManager;
			AClickable() : _enabled(true) {};
			virtual ~AClickable() {};
			virtual void OnHover();
			virtual void OnClick(const int &button, const int &action);
			virtual void OnHoverLeave();
			virtual bool IsHover(const Vector2Float &mousePos);

			virtual void ChangeEnableState(bool newState);

			bool currentlyHovered = false;
			bool IsEnabled()
			{
				return this->_enabled;
			};

			virtual void OnEnable();
			virtual void OnDisable();

			Vox::Utils::CallBacksManager<const int &, const int &> onClickCallbacks;

		protected:
			Vox::Utils::CallBacksManager<> _onHoverCallbacks;
			Vox::Utils::CallBacksManager<> _onHoverLeaveCallbacks;
			Vox::Utils::CallBacksManager<const bool &> _onEnableStatusChangeCallbacks;

			static bool _hoverConsummed;
			static bool _clickConsummed;
			bool _enabled;

		private:
			static void ResetHoverState()
			{
				AClickable::_hoverConsummed = false;
			}
			static void ResetClickState()
			{
				AClickable::_clickConsummed = false;
			}
	};

	inline void AClickable::OnHover()
	{
		if (_enabled == false)
			return;
		this->_onHoverCallbacks.Notify();
	}

	inline void AClickable::OnClick(const int &button, const int &action)
	{
		if (_enabled == false || currentlyHovered == false || _clickConsummed == true)
			return;
		_clickConsummed = true;
		this->onClickCallbacks.Notify(button, action);
	}

	inline void AClickable::OnHoverLeave()
	{
		if (_enabled == false)
			return;
		this->_onHoverLeaveCallbacks.Notify();
	}

	inline bool AClickable::IsHover(const Vector2Float &mousePos)
	{
		if (this->_enabled == false)
			return false;

		if ((mousePos[0] >= this->_pos[0] && mousePos[0] <= this->_pos[0] + this->_size[0]) &&
			(mousePos[1] >= this->_pos[1] && mousePos[1] <= this->_pos[1] + this->_size[1]) &&
			AClickable::_hoverConsummed == false)
		{
			AClickable::_hoverConsummed = true;
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
	}

	inline void AClickable::ChangeEnableState(bool newState)
	{
		if (newState == this->_enabled)
			return;
		this->_enabled = newState;
		if (this->_enabled)
			OnEnable();
		else
			OnDisable();
	};

	inline void AClickable::OnEnable()
	{
		this->_onEnableStatusChangeCallbacks.Notify(true);
	}

	inline void AClickable::OnDisable()
	{
		this->_onEnableStatusChangeCallbacks.Notify(false);
	}

} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __ACLICKABLE_HPP__