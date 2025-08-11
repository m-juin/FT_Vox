#ifndef __PIPELINEMANAGER_HPP__
#define __PIPELINEMANAGER_HPP__

#include <vulkan/vulkan.h>

namespace Vox::Front::Rendering
{
	class SwapChain;
	class Device;

	class PipelineManager
	{
		private:
			VkRenderPass _renderPass;
			void CreateRenderPass(SwapChain *swapChain, Device *device);

			Device *_device;
			PipelineManager();

		public:
			PipelineManager(SwapChain *swapChain, Device *device);
			~PipelineManager();

			VkRenderPass &GetRenderPass()
			{
				return this->_renderPass;
			};
	};
} // namespace Vox::Front::Rendering
#endif // __PIPELINEMANAGER_HPP__