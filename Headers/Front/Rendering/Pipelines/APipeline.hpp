#ifndef __APIPELINE_HPP__
#define __APIPELINE_HPP__

#include <vector>
#include <vulkan/vulkan.h>

namespace Vox::Front::Rendering::Pipelines
{
	class APipeline
	{
		public:
			virtual ~APipeline();
			VkPipeline &GetPipeline()
			{
				return this->_instance;
			};
			VkPipelineLayout &GetLayout()
			{
				return this->_layout;
			};

		protected:
			VkPipelineLayout _layout;
			VkPipeline _instance;

			VkShaderModule CreateShaderModule(const std::vector<char> &code);
			virtual void CreatePipeline(VkDescriptorSetLayout &layout) = 0;
			APipeline();
	};
} // namespace Vox::Front::Rendering::Pipelines

#endif // __APIPELINE_HPP__