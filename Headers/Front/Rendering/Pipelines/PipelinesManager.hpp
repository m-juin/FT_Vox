#ifndef __PIPELINESMANAGER_HPP__
#define __PIPELINESMANAGER_HPP__

#include <vulkan/vulkan.h>

#include "Utils/Singleton.hpp"

namespace Vox::Front::Rendering::Pipelines
{
	class PipelinesManager : public Vox::Utils::Singleton<PipelinesManager>
	{
		private:
			friend class Vox::Utils::Singleton<PipelinesManager>;
			VkRenderPass _renderPass;
			void CreateRenderPass();

			PipelinesManager();

		public:
			~PipelinesManager();

			VkRenderPass &GetRenderPass()
			{
				return this->_renderPass;
			};
	};
} // namespace Vox::Front::Rendering::Pipelines
#endif // __PIPELINESMANAGER_HPP__