#ifndef __STATICGUIPIPELINE_HPP__
#define __STATICGUIPIPELINE_HPP__

#include "Front/Rendering/Pipelines/APipeline.hpp"

namespace Vox::Front::Rendering::Pipelines
{
    class StaticGUIPipeline : public APipeline
    {
        
        public:
            StaticGUIPipeline();
            ~StaticGUIPipeline();

            void CreatePipeline(VkDescriptorSetLayout &layout) override;
    
        private:
            /* private */
    
    };
} // namespace Vox::Front::Rendering::Pipelines


#endif // __STATICGUIPIPELINE_HPP__