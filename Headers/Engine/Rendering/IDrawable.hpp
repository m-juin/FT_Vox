#ifndef __IDRAWABLE_HPP__
#define __IDRAWABLE_HPP__

namespace Vox::Engine::Rendering
{
    class IDrawable
    {
        public:
            IDrawable() {};
            virtual ~IDrawable() {};
            virtual bool Draw() = 0;    
        private:
            /* private */
    
    };
} // namespace Vox::Engine::Rendering


#endif // __IDRAWABLE_HPP__