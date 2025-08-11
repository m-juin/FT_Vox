#ifndef __WINDOW_HPP__
#define __WINDOW_HPP__

#include <cstdint>
#include <vulkan/vulkan.h>

#include "Utils/Singleton.hpp"

class GLFWwindow;

namespace Vox::Front
{
	class Window : public Vox::Utils::Singleton<Window>
	{
		friend class Vox::Utils::Singleton<Window>;
		private:
			GLFWwindow *_window;
			VkSurfaceKHR _surface;
			/* private */
			Window(uint16_t width = 1920, uint16_t height = 1080);
		public:
			GLFWwindow *GetWindow()
			{
				return this->_window;
			};
			VkSurfaceKHR &GetSurface()
			{
				return this->_surface;
			};
			~Window();
			void SetupSurface();
	};

} // namespace Vox::Front

#endif // __WINDOW_HPP__