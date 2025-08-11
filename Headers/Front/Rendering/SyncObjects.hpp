#ifndef __SYNCOBJECTS_HPP__
#define __SYNCOBJECTS_HPP__

#include <vulkan/vulkan.h>

#include <vector>

namespace Vox
{
	namespace Front
	{
		namespace Rendering
		{
            constexpr uint16_t MAX_FRAMES_IN_FLIGHT = 2;

            class Device;

			class SyncObjects
			{
				private:

					std::vector<VkSemaphore> _imageAvailableSemaphores;
					std::vector<VkSemaphore> _renderFinishedSemaphores;
					std::vector<VkFence> _inFlightFences;

                    uint8_t _currentFrame = 0;

                    Device *_device;

				public:

                    uint8_t GetCurrentFrame() {return this->_currentFrame;};

					void GoToNextFrame();

					std::vector<VkFence> GetFences() {return this->_inFlightFences;};
					VkSemaphore &GetCurrentImageSemaphore() {return this->_imageAvailableSemaphores[this->_currentFrame];};
					VkSemaphore &GetCurrentRenderFinishedSemaphore() {return this->_renderFinishedSemaphores[this->_currentFrame];};
					VkFence &GetCurrentFence() {return this->_inFlightFences[this->_currentFrame];};
					SyncObjects(Device *device);
					~SyncObjects();
			};
		} // namespace Rendering

	} // namespace Front

} // namespace Vox

#endif // __SYNCOBJECTS_HPP__