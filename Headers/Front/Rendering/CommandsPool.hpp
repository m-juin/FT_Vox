#ifndef __COMMANDSPOOL_HPP__
#define __COMMANDSPOOL_HPP__

#include <vulkan/vulkan.h>

#include <vector>

namespace Vox
{
    namespace Front
    {
        namespace Rendering
        {
            class Device;
            class SwapChain;
            class PipelineManager;

            class CommandsPool
            {            
                private:
                    VkCommandPool _pool;

                    std::vector<VkCommandBuffer> _buffers;

                    Device *_device;
                    /* private */
                    CommandsPool();
                public:
                    void CreateCommandBuffer();
                    void ResetBuffer(uint32_t bufferIndex);

                    void BeginRecord(uint32_t imageIndex, uint32_t frame, SwapChain *swap, PipelineManager *pipelineManager);
                    void EndRecord(uint32_t frame);

                    VkCommandPool &GetPool() {return this->_pool;};
                    VkCommandBuffer &GetBuffer(uint8_t index) {return this->_buffers[index];};

                    CommandsPool(Device *device, VkSurfaceKHR &surface);
                    ~CommandsPool();
            
            };
        } // namespace Rendering
        
    } // namespace Front
    
} // namespace Vox


#endif // __COMMANDSPOOL_HPP__