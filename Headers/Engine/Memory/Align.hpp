#pragma once

#include <cstdint>
#include <cstddef>

namespace Vox::Engine::Memory
{
	inline size_t Align(size_t size, size_t alignment = 16)
	{
		/***
		 * (size + alignment - 1) Since operation always round to lower.
		 * ~(alignment - 1) Create bitmask for the alignement - 1 (16: 16 - 1 = 15; 15 bit = 0000 1111) and we negate it
		 * (~)(15: 0000 1111 > 1111 0000) & (AND) operation return 1 if both number bit is 1 so: (14 + 16 - 1 (29): 0001
		 * 1101 & ~(16 - 1): 1111 0000) result = 0001 0000 (16)
		 ***/
		return (size + alignment - 1) & ~(alignment - 1);
	}
} // namespace Vox::Engine::Memory