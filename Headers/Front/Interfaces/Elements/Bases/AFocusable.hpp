#ifndef __AFOCUSABLE_HPP__
#define __AFOCUSABLE_HPP__

namespace Vox::Front::Interfaces::Elements::Bases
{
    class AFocusable
    {
        public:
            friend class Vox::Front::Interfaces::InterfacesManager;
            AFocusable() : _isFocused(false) {};
            virtual ~AFocusable()
            {
                if (GetFocused() == this)
                    ResetFocusElement();
            };

            static inline AFocusable *GetFocused()
            {
                return AFocusable::_focusedElement;
            }

        protected:

            bool _isFocused;

            static AFocusable *_focusedElement;

            static bool _focusConsummed;

			static inline void ResetFocusConsumtion()
			{
				AFocusable::_focusConsummed = false;
			}

            static inline void ResetFocusElement()
            {
                AFocusable::_focusedElement = nullptr;
            }

            static inline bool SetFocusElement(AFocusable *focused)
            {
                if (AFocusable::_focusConsummed == true) return false;
                if (focused != nullptr)
                {
                    _focusConsummed = true;
                    if (AFocusable::_focusedElement != nullptr && AFocusable::_focusedElement != focused)
                        AFocusable::_focusedElement->_isFocused = false;
                    focused->_isFocused = true;
                }
                AFocusable::_focusedElement = focused;
                return true;
            }
            /* private */
    
    };
}

#endif // __AFOCUSABLE_HPP__