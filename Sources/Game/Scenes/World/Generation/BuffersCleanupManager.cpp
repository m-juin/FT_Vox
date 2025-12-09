#include "Game/Scenes/World/Generation/BuffersCleanupManager.hpp"

#include "Front/Rendering/Device.hpp"
#include "Front/Rendering/SyncObjects.hpp"
#include "Utils/TracyUtils.hpp"

#include <algorithm>

namespace Vox::World::Generation
{
	BuffersCleanupManager::BuffersCleanupDatas::BuffersCleanupDatas(
		Vox::Front::Rendering::Utils::Buffers::ABuffer *buffer_)
		: buffer(buffer_), framecount(0)
	{
	}

	BuffersCleanupManager::BuffersCleanupManager() {}

	void BuffersCleanupManager::RequestCleanup(Vox::Front::Rendering::Utils::Buffers::ABuffer *buffer)
	{
        auto it = std::find_if(this->toDelete.begin(), this->toDelete.end(), [buffer](BuffersCleanupDatas &data)
        {
            return buffer == data.buffer;
        });
        if (it == this->toDelete.end())
		    this->toDelete.push_back({buffer});
		LoggerLib::LogDebug("Adding Buffer Memory To Delete: ", this->toDelete.size());
	}

	void BuffersCleanupManager::ProcessFrameCleanup()
	{
		for (auto it = this->toDelete.begin(); it < this->toDelete.end();)
		{
			if (it->framecount >= MAX_FRAMES_IN_FLIGHT)
			{
                it->buffer->CleanUp();
				delete it->buffer;
				this->toDelete.erase(it);
			}
			else
			{
                it->framecount++;
				it++;
			}
		}
		LoggerLib::LogDebug("Buffer Memory To Delete: ", this->toDelete.size());
	}
} // namespace Vox::World::Generation