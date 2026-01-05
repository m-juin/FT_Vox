#ifndef __BUFFERMEMORYMANAGER_HPP__
#define __BUFFERMEMORYMANAGER_HPP__

#include "Front/Rendering/Utils/Buffers/DynamicBuffer.hpp"
#include "Utils/Optional.hpp"

#include <memory>
#include <unordered_map>
#include <vector>

#include "Utils/ALateUpdatable.hpp"

namespace Vox::Game::Rendering
{
	using dbuffer = Front::Rendering::Utils::Buffers::DynamicBuffer;
	constexpr uint16_t MAX_BUFFER_COUNT = 128 * 128;

	const std::vector<uint64_t> avalaibleDeviceSize{
		32 * 32, 64 * 64, 128 * 128, 256 * 256, 512 * 512,
	};

	struct BufferMemory
	{
			std::unique_ptr<dbuffer> buffer;
			VkDeviceSize bufferSize;
			const VkBufferUsageFlagBits usage;
			uint8_t toRefreshFrame;
			BufferMemory(std::unique_ptr<dbuffer> buf, VkDeviceSize size, VkBufferUsageFlagBits use)
				: buffer(std::move(buf)), bufferSize(size), usage(use)
			{
			}
	};

	class BufferMemoryManager : public virtual Vox::Utils::ALateUpdatable
	{
		private:
			std::vector<std::shared_ptr<BufferMemory>> bufferPool;
			std::vector<std::shared_ptr<BufferMemory>> freePool;
			std::vector<std::shared_ptr<BufferMemory>> toDeletePool;
			VkDeviceSize GetNextSize(VkDeviceSize);

			std::vector<std::shared_ptr<BufferMemory>>::iterator FindAvalaibleMemory(VkDeviceSize,
																					 VkBufferUsageFlagBits);

		public:
			std::shared_ptr<BufferMemory> GetBufferOfSize(VkDeviceSize, VkBufferUsageFlagBits);
			void ReleaseBuffer(std::shared_ptr<BufferMemory>);
			BufferMemoryManager();
			~BufferMemoryManager();

			/* private */
	};
} // namespace Vox::Game::Rendering

#endif // __BUFFERMEMORYMANAGER_HPP__