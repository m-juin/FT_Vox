#ifndef __ASCROLLABLE_HPP__
#define __ASCROLLABLE_HPP__

#include "./AClickable.hpp"

#include "Utils/CallBacksManager.hpp"

namespace Vox::Front::Interfaces::Elements::Bases
{
	class AScrollable :  public virtual AClickable
	{
		public:
			AScrollable() {}

            virtual void OnScroll(const double &xOff, const double &yOff) { if (this->currentlyHovered) this->onScrollCallBacks.Notify(xOff, yOff);};

			virtual ~AScrollable() {};

			Vox::Utils::CallBacksManager<const double &, const double &> onScrollCallBacks;

		private:
			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __ASCROLLABLE_HPP__