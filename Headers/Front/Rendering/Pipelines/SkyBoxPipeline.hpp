#ifndef __SKYBOX_PIPELINE_HPP__
#define __SKYBOX_PIPELINE_HPP__

#include "./APipeline.hpp"

namespace Vox::Front::Rendering::Pipelines
{
	class SkyBoxPipeline : public APipeline
	{

		public:
			SkyBoxPipeline();
			~SkyBoxPipeline();

			void CreatePipeline() override;
			void InitSet(std::vector<VkBuffer> buffers, VkDeviceSize size);
			void CreateSet(VkDescriptorPool &descPool) override;

		private:
			void CreateSetLayout() override;
			/* private */
	};
}


#endif // __SKYBOX_PIPELINE_HPP__