#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <cstdint>
#include <vulkan/vulkan.h>

class GLFWwindow;

namespace Vox
{
    namespace Front
    {
        class Window
        {        
            private:
                GLFWwindow* _window;
                VkSurfaceKHR _surface;
                /* private */
                Window(void);

            public:

                GLFWwindow* GetWindow() {return this->_window;};
                VkSurfaceKHR &GetSurface() {return this->_surface;};

                void SetupSurface(VkInstance &instance);

                Window(uint16_t width = 1920, uint16_t height = 1080);
                ~Window();
        
        };
    } // namespace Front
    
} // namespace Vox


#endif // __WINDOW_HPP__