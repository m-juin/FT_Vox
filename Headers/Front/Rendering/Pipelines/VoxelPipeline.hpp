#ifndef __VOXELPIPELINE_HPP__
#define __VOXELPIPELINE_HPP__

#include "./APipeline.hpp"

namespace Vox::Front::Rendering::Pipelines
{
	class VoxelPipeline : public APipeline
	{

		public:
			VoxelPipeline();
			~VoxelPipeline();

			void CreatePipeline() override;
			void InitSet(std::vector<VkBuffer> buffers, VkDeviceSize size);
			void CreateSet(VkDescriptorPool &descPool) override;

		private:
			void CreateSetLayout() override;
			/* private */
	};
} // namespace Vox::Front::Rendering::Pipelines

#endif // __VOXELPIPELINE_HPP__