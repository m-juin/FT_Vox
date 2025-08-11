#ifndef __COMMANDSPOOL_HPP__
#define __COMMANDSPOOL_HPP__

#include <vulkan/vulkan.h>

#include <vector>

#include "Utils/Singleton.hpp"

namespace Vox::Front::Rendering
{
	class CommandsPool : public Vox::Utils::Singleton<CommandsPool>
	{
		friend class Vox::Utils::Singleton<CommandsPool>;
		private:
			VkCommandPool _pool;

			std::vector<VkCommandBuffer> _buffers;
			/* private */
			CommandsPool();

		public:
			void CreateCommandBuffer();
			void ResetBuffer(uint32_t bufferIndex);

			void BeginRecord(uint32_t imageIndex, uint32_t frame);
			void EndRecord(uint32_t frame);

			VkCommandPool &GetPool()
			{
				return this->_pool;
			};
			VkCommandBuffer &GetBuffer(uint8_t index)
			{
				return this->_buffers[index];
			};

			CommandsPool(VkSurfaceKHR &surface);
			~CommandsPool();
	};

} // namespace Vox::Front::Rendering

#endif // __COMMANDSPOOL_HPP__