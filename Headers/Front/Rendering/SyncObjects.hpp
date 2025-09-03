#ifndef __SYNCOBJECTS_HPP__
#define __SYNCOBJECTS_HPP__

#include <vulkan/vulkan.h>

#include <vector>
#include "Utils/Singleton.hpp"

constexpr uint16_t MAX_FRAMES_IN_FLIGHT = 2;

namespace Vox::Front::Rendering
{

	class SyncObjects : public Vox::Utils::Singleton<SyncObjects>
	{
		friend class Vox::Utils::Singleton<SyncObjects>;
		private:
			std::vector<VkSemaphore> _imageAvailableSemaphores;
			std::vector<VkSemaphore> _renderFinishedSemaphores;
			std::vector<VkFence> _inFlightFences;

			uint8_t _currentFrame = 0;

		public:
			uint8_t GetCurrentFrame()
			{
				return this->_currentFrame;
			};

			uint8_t GetNextFrame()
			{
				return (this->_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
			};

			void GoToNextFrame();

			std::vector<VkFence> GetFences()
			{
				return this->_inFlightFences;
			};
			VkSemaphore &GetCurrentImageSemaphore()
			{
				return this->_imageAvailableSemaphores[this->_currentFrame];
			};
			VkSemaphore &GetCurrentRenderFinishedSemaphore()
			{
				return this->_renderFinishedSemaphores[this->_currentFrame];
			};
			VkFence &GetCurrentFence()
			{
				return this->_inFlightFences[this->_currentFrame];
			};
			SyncObjects();
			~SyncObjects();
	};
} // namespace Vox::Front::Rendering

#endif // __SYNCOBJECTS_HPP__