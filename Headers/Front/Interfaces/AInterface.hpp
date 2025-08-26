#ifndef __AINTERFACE_HPP__
#define __AINTERFACE_HPP__

#include "Front/Interfaces/Elements/Bases/AContainer.hpp"


namespace Vox::Front::Interfaces
{
	class AInterface : public Elements::Bases::AContainer
	{
		public:
			AInterface(Elements::Vector2 pos = {0, 0}, Elements::Vector2 size = {0, 0}, bool isEnabled = true) : AContainer(pos, size), _isEnabled(isEnabled) {};
			virtual ~AInterface() {};

			virtual void Render() = 0;
			virtual void SetPos(const Elements::Vector2 newPos) = 0;
			virtual void SetSize(const Elements::Vector2 newSize) = 0;

			bool IsEnabled() {return this->_isEnabled;};
			void ChangeEnableStatus(bool newState)
			{
				this->_isEnabled = newState;
			};
		
			private:
				bool _isEnabled;
			/* private */
	};
} // namespace Vox::Front::Interfaces

#endif // __AINTERFACE_HPP__