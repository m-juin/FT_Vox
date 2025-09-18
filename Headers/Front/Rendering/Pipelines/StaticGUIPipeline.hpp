#ifndef __STATICGUIPIPELINE_HPP__
#define __STATICGUIPIPELINE_HPP__

#include "Front/Rendering/Pipelines/APipeline.hpp"

#include "Front/Utils/TexturesData.hpp"

#include <array>
namespace Vox::Front::Rendering::Pipelines
{
    class StaticGUIPipeline : public APipeline
    {
        
        public:
            StaticGUIPipeline();
            ~StaticGUIPipeline();

            void CreatePipeline() override;
			void CreateSet(VkDescriptorPool &descPool) override;
            void UpdateSet(const size_t &index);
        private:

            std::array<VkWriteDescriptorSet, 3> _descriptorWrites;
            std::array<VkDescriptorImageInfo, Vox::Front::Utils::TexturesData::MAX_DYNAMIC_TEXTURES> _dynamicInfos;
            void CreateSetLayout() override;
            /* private */
    
    };
} // namespace Vox::Front::Rendering::Pipelines


#endif // __STATICGUIPIPELINE_HPP__