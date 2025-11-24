#ifndef __BUFFERMANAGER_HPP__
#define __BUFFERMANAGER_HPP__

#include <bitset>

#include "Game/Scenes/World/Utils/Defines.hpp"

#include "LoggerLib/UtilityFunctions.hpp"

namespace Vox::Game::Generation
{
	class BufferManager
	{
		public:
			BufferManager()
			{
				_avalaibleBuffers.flip();
			};
			~BufferManager() {};
			inline uint32_t ReserveBuffer()
			{
				uint32_t index = _avalaibleBuffers._Find_first();
				if (index != _avalaibleBuffers.size())
					_avalaibleBuffers.set(index, false);
				return index;
			}

			inline void ReleaseBuffer(const uint32_t &index)
			{
				if (index >= Vox::Game::Utils::Defines::CHUNCK_BUFFER_AMOUNT)
				{
					LoggerLib::LogWarning("Buffer index ", index, " too big.");
					return;
				}
				else if (_avalaibleBuffers._Unchecked_test(index) == true)
				{
					LoggerLib::LogWarning("Buffer at index ", index, " already released.");
					return;
				}
                _avalaibleBuffers._Unchecked_set(index, true);
			}

		private:
			std::bitset<Vox::Game::Utils::Defines::CHUNCK_AMOUNT> _avalaibleBuffers;
			/* private */
	};
} // namespace Vox::Game::Generation

#endif // __BUFFERMANAGER_HPP__