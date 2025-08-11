#ifndef __VULKANMANAGER_HPP__
#define __VULKANMANAGER_HPP__

#include <vulkan/vulkan.h>

#include <vector>
#include "Utils/Singleton.hpp"


namespace Vox::Front::Rendering
{
	namespace Images
	{
		class DepthImage;
	} // namespace Images

	class VulkanManager : public Vox::Utils::Singleton<VulkanManager>
	{
        friend class Vox::Utils::Singleton<VulkanManager>;
		private:
			VkInstance _instance;
			Images::DepthImage *_depthImage;

			VkDebugUtilsMessengerEXT _debugMessenger;

			std::vector<const char *> GetRequiredExtensions();

		public:
			VkInstance &GetVkInstance()
			{
				return this->_instance;
			};
			Images::DepthImage *GetDepthImage()
			{
				return this->_depthImage;
			};

			void SetDepthImage(Images::DepthImage *image)
			{
				this->_depthImage = image;
			};

			VulkanManager();
			~VulkanManager();
	};
} // namespace Vox::Front::Rendering

#endif // __VULKANMANAGER_HPP__