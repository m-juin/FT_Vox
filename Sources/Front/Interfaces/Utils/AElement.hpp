#ifndef __AELEMENT_HPP__
#define __AELEMENT_HPP__

#include <cstdint>

namespace Vox::Front::Interfaces::BaseClass
{
    class AElement
    {
        public:
            virtual void Draw() = 0;
            virtual ~AElement() {};
        protected:
            AElement() {};
        private:
            /* private */
    
    };
} // namespace Vox::Front::Interfaces::Utils


#endif // __AELEMENT_HPP__