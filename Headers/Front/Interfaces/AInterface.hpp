#ifndef __AINTERFACE_HPP__
#define __AINTERFACE_HPP__

#include "Front/Interfaces/Elements/Bases/AContainer.hpp"
#include "Front/Interfaces/Elements/Bases/AScrollable.hpp"


namespace Vox::Front::Interfaces
{
	class AInterface : public virtual Elements::Bases::AContainer, public virtual Elements::Bases::AScrollable
	{
		public:
			AInterface(Elements::Vector2 pos = {0, 0}, Elements::Vector2 size = {0, 0}, bool isEnabled = true) : AElement(pos, size), AClickable(), AContainer(), AScrollable()
			{
				_enabled = isEnabled;
				this->onScrollCallBacks.AddCallBack([this](const double &xOff, const double &yOff){
					for(auto &elem : this->_content)
					{
						auto converted = dynamic_cast<AScrollable *>(elem.elem.get());
						if (converted == nullptr) continue;
						if (converted->currentlyHovered)
						{
							converted->OnScroll(xOff, yOff);
						}
					}
				});
			};
			virtual ~AInterface() {};

			virtual void OnScroll(const double &xOff, const double &yOff) override {this->onScrollCallBacks.Notify(xOff, yOff);};

			virtual void Render() = 0;
		
			private:
			/* private */
	};
} // namespace Vox::Front::Interfaces

#endif // __AINTERFACE_HPP__