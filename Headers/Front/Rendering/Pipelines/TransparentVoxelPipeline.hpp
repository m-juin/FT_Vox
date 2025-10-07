#ifndef __TRANSPARENTVOXELPIPELINE_HPP__
#define __TRANSPARENTVOXELPIPELINE_HPP__

#include "./APipeline.hpp"

namespace Vox::Front::Rendering::Pipelines
{
	class TransparentVoxelPipeline : public APipeline
	{

		public:
			TransparentVoxelPipeline();
			~TransparentVoxelPipeline();

			void CreatePipeline() override;
			void InitSet(std::vector<VkBuffer> buffers, VkDeviceSize size);
			void CreateSet(VkDescriptorPool &descPool) override;

		private:
			void CreateSetLayout() override;
			/* private */
	};
} // namespace Vox::Front::Rendering::Pipelines

#endif // __TRANSPARENTVOXELPIPELINE_HPP__