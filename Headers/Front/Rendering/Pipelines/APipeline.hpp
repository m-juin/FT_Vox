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
			}
			VkPipelineLayout &GetLayout()
			{
				return this->_layout;
			}

			VkDescriptorSetLayout &GetSetLayout()
			{
				return this->_slayout;
			}
			VkDescriptorSet &GetSet(const size_t &frame = 0)
			{
				return this->_set[frame];
			}

		protected:
			VkDescriptorSetLayout _slayout = VK_NULL_HANDLE;
			std::vector<VkDescriptorSet> _set;

			VkPipelineLayout _layout;
			VkPipeline _instance;

			VkShaderModule CreateShaderModule(const std::vector<char> &code);
			virtual void CreatePipeline() = 0;
			virtual void CreateSetLayout() = 0;
			virtual void CreateSet(VkDescriptorPool &descPool) = 0;
			
			APipeline();
	};
} // namespace Vox::Front::Rendering::Pipelines

#endif // __APIPELINE_HPP__