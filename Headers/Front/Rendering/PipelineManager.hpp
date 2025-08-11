#ifndef __PIPELINEMANAGER_HPP__
#define __PIPELINEMANAGER_HPP__

#include <vulkan/vulkan.h>

#include "Utils/Singleton.hpp"

namespace Vox::Front::Rendering
{

	class PipelineManager : public Vox::Utils::Singleton<PipelineManager>
	{
		private:
			friend class Vox::Utils::Singleton<PipelineManager>;
			VkRenderPass _renderPass;
			void CreateRenderPass();

			PipelineManager();

		public:
			~PipelineManager();

			VkRenderPass &GetRenderPass()
			{
				return this->_renderPass;
			};
	};
} // namespace Vox::Front::Rendering
#endif // __PIPELINEMANAGER_HPP__