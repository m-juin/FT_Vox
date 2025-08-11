#ifndef __VULKANMANAGER_HPP__
#define __VULKANMANAGER_HPP__

#include <vulkan/vulkan.h>

#include <vector>

namespace SplineDesigner
{
    namespace Front
    {
        namespace Rendering
        {
            class Device;
            class SwapChain;
            class DescriptorPool;
            class PipelineManager;
            class SyncObjects;
            class CommandsPool;

            namespace Images { class DepthImage; } // namespace Images
            

            class VulkanManager
            {

                private:
                    VkInstance _instance;
                    Device *_device;
                    SwapChain *_swapChain;
                    DescriptorPool *_descPool;
                    PipelineManager *_pipelineManager;
                    SyncObjects *_syncObjects;
                    CommandsPool *_commandsPool;
                    Images::DepthImage *_depthImage;

                    VkDebugUtilsMessengerEXT _debugMessenger;

                    std::vector<const char *> GetRequiredExtensions();

                public:
                    VkInstance &GetVkInstance() {return this->_instance;};

                    SwapChain *GetSwapChain() {return this->_swapChain;};
                    Device *GetDevice() {return this->_device;};
                    DescriptorPool *GetDescPool() {return this->_descPool;};
                    PipelineManager *GetPipelineManager() {return this->_pipelineManager;};
                    Images::DepthImage *GetDepthImage() {return this->_depthImage;};
                    SyncObjects *GetSyncObjects() {return this->_syncObjects;};
                    CommandsPool *GetCommandsPool() {return this->_commandsPool;};

                    void SetSwapChain(SwapChain *swapChain) {this->_swapChain = swapChain;};
                    void SetDevice(Device *device) {this->_device = device;};
                    void SetDescPool(DescriptorPool *descPool) {this->_descPool = descPool;};
                    void SetPipelineManager(PipelineManager *manager) {this->_pipelineManager = manager;};
                    void SetDepthImage(Images::DepthImage *image) {this->_depthImage = image;};
                    void SetSyncObjects(SyncObjects *objects) {this->_syncObjects = objects;};
                    void SetCommandsPool(CommandsPool *pool) {this->_commandsPool = pool;};

                    VulkanManager();
                    ~VulkanManager();
            
            };
        } // namespace Rendering
        
    } // namespace Front
    
} // namespace SplineDesigner


#endif // __VULKANMANAGER_HPP__