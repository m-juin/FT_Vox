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

            void CreatePipeline() override;
			void CreateSet(VkDescriptorPool &descPool) override;
            
        private:
            void CreateSetLayout() override;
            /* private */
    
    };
} // namespace Vox::Front::Rendering::Pipelines


#endif // __STATICGUIPIPELINE_HPP__