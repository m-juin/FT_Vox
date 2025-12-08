#ifndef __BUFFERSCLEANUPMANAGER_HPP__
#define __BUFFERSCLEANUPMANAGER_HPP__

#include <vector>

#include "Front/Rendering/Utils/Buffers/ABuffer.hpp"

namespace Vox::World::Generation
{
    class BuffersCleanupManager
    {
        struct BuffersCleanupDatas
        {
            Vox::Front::Rendering::Utils::Buffers::ABuffer *buffer;
            uint8_t framecount;
            BuffersCleanupDatas(Vox::Front::Rendering::Utils::Buffers::ABuffer *);
        };

        public:
            BuffersCleanupManager();
            ~BuffersCleanupManager() {};
            void RequestCleanup(Vox::Front::Rendering::Utils::Buffers::ABuffer *buffer);
            void ProcessFrameCleanup();
        private:
            std::vector<BuffersCleanupDatas> toDelete;
    
    };
} // namespace Vox::World::Generation
#endif // __BUFFERSCLEANUPMANAGER_HPP__