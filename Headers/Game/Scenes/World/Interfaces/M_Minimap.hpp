#ifndef __M_MINIMAP_HPP__
#define __M_MINIMAP_HPP__

#include <vector>

// #include "MathGraphicalLib/Vectors/Vector2.hpp"
// #include "MathGraphicalLib/Vectors/Vector3.hpp"

#include "MathGraphicalLib/Vectors/Defines.hpp"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace Vox::Game::Scenes::World::Interfaces
{
	using namespace MGL::Vectors::Types;
	class M_Minimap
	{
		public:
			M_Minimap(MGL::Vectors::Vector2<size_t> imgSize);
			~M_Minimap();

			void Start();
			void Stop();

			void RequestUpdate(Vector3float newPlayerPos, const uint32_t &newSeed, const float &newScale, const uint8_t &newMap);
			const std::vector<uint8_t> &GetLatestBuffer();

		private:
			/* private */
			std::mutex _processMutex;
			std::thread _process;
			std::atomic<bool> _running{false};

			MGL::Vectors::Vector2<size_t> _imgSize;

			std::vector<uint8_t> _frontBuffer;
			std::vector<uint8_t> _backBuffer;

			std::condition_variable _cv;
			std::mutex _cvMutex;
			bool _hasRequest = false;
			MGL::Vectors::Vector3<float> _requestedPos;
			float _requestedScale;
			uint32_t _requestedSeed;
			uint8_t _requestedMap;

			void WorkerLoop();
			void Generate(std::vector<uint8_t> &targetBuffer, Vector2Int playerPos, uint32_t seed, float scale, uint8_t map);
	};

} // namespace Vox::Game::Scenes::World::Interfaces

#endif // __M_MINIMAP_HPP__