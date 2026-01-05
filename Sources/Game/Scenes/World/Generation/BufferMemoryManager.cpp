#include "Game/Scenes/World/Generation/BufferMemoryManager.hpp"

#include <algorithm>

#include "LoggerLib/UtilityFunctions.hpp"

namespace Vox::Game::Rendering
{
	VkDeviceSize BufferMemoryManager::GetNextSize(VkDeviceSize requiredSize)
	{
		for (const auto &size : avalaibleDeviceSize)
		{
			if (size >= requiredSize)
			{
				return size;
			}
		}
		return 0;
	}

	std::vector<std::shared_ptr<BufferMemory>>::iterator BufferMemoryManager::FindAvalaibleMemory(
		VkDeviceSize requiredSize, VkBufferUsageFlagBits usage)
	{

		auto it = std::find_if(freePool.begin(), freePool.end(),
							   [requiredSize, usage](const std::shared_ptr<BufferMemory> &memory)
							   { return memory->bufferSize == requiredSize && memory->usage == usage; });

		if (it != this->freePool.end())
		{
			// LoggerLib::LogDebug("FOUND buffer at index ", std::distance(freePool.begin(), it));
			return it;
		}

		LoggerLib::LogError("No avalaible memory found. Creating new one ...");

		if (bufferPool.size() >= MAX_BUFFER_COUNT)
		{
			LoggerLib::LogWarning("Can't create buffer. Max amount reached. Please wait.");
			return it;
		}

		std::shared_ptr<BufferMemory> ptr =
			std::make_shared<BufferMemory>(std::make_unique<dbuffer>(2, requiredSize, usage), requiredSize, usage);
		bufferPool.push_back(ptr);
		size_t oldSize = freePool.size();
		freePool.push_back(ptr);
		ptr->buffer->Create(nullptr);
		return freePool.begin() + oldSize;
	}

	std::shared_ptr<BufferMemory> BufferMemoryManager::GetBufferOfSize(VkDeviceSize requiredSize,
																	   VkBufferUsageFlagBits usage)
	{
		// LoggerLib::LogDebug("FreeBuffer Size = ", this->freePool.size());
		VkDeviceSize bufferSize = GetNextSize(requiredSize);
		LoggerLib::LogInfo("For memory of size ", requiredSize, " find closest memory size of ", bufferSize);

		if (bufferSize == 0)
		{
			LoggerLib::LogError("Memory size too big to be found.");
			return nullptr;
		}
		std::vector<std::shared_ptr<BufferMemory>>::iterator it = FindAvalaibleMemory(bufferSize, usage);

		if (it == this->freePool.end())
		{
			return nullptr;
		}

		this->freePool.erase(it);
		return *it;
	}

	void BufferMemoryManager::ReleaseBuffer(std::shared_ptr<BufferMemory> memory)
	{
		// memory->buffer->Clear();
		// this->freePool.push_back(memory);
		memory->toRefreshFrame = 2;
		this->toDeletePool.push_back(memory);
	}

	BufferMemoryManager::BufferMemoryManager()
	{
		this->onLateUpdate.AddCallBack(
			[this]()
			{
				for (auto it = this->toDeletePool.begin(); it != this->toDeletePool.end();)
				{
					if ((*it)->toRefreshFrame == 0)
					{
						this->toDeletePool.erase(it);
						(*it)->buffer->Clear();
						this->freePool.push_back((*it));
					}
					else
					{
						(*it)->toRefreshFrame -= 1;
						++it;
					}
				}
				// this->toDeletePool.clear();
			});
	}

	BufferMemoryManager::~BufferMemoryManager() {}
} // namespace Vox::Game::Rendering