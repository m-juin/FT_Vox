#include "Game/Scenes/World/Interfaces/M_Minimap.hpp"

#include <iostream>

#include "Game/Scenes/World/Generation/BiomesPerlin.hpp"

#include "LoggerLib/UtilityFunctions.hpp"

namespace Vox::Game::Scenes::World::Interfaces
{
	M_Minimap::M_Minimap(MGL::Vectors::Vector2<size_t> imgSize) : _imgSize(imgSize)
	{
		this->_frontBuffer.resize(imgSize[0] * imgSize[1] * 4);
		this->_backBuffer.resize(imgSize[0] * imgSize[1] * 4);
	}

	M_Minimap::~M_Minimap()
	{
		this->Stop();
	}

	void M_Minimap::Start()
	{
		if (this->_running == true)
		{
			LoggerLib::LogWarning("Trying to start minimap thread who is already started.");
			return;
		}
		this->_running = true;
		this->_process = std::thread(&M_Minimap::WorkerLoop, this);
	}

	void M_Minimap::Stop()
	{
		if (this->_running == false)
		{
			LoggerLib::LogWarning("Trying to stop minimap thread who is already stopped.");
			return;
		}
		this->_running = false;
		this->_cv.notify_all();
		if (this->_process.joinable() == true)
			this->_process.join();
	}

	void M_Minimap::RequestUpdate(MGL::Vectors::Vector3<float> newPlayerPos, const uint32_t &newSeed, const float &newScale, const uint8_t &newMap)
	{
		{
			std::lock_guard<std::mutex> lock(_processMutex);
			if (newPlayerPos == this->_requestedPos && newSeed == this->_requestedSeed && newScale == this->_requestedScale && newMap == this->_requestedMap)
				return;
			this->_requestedPos = newPlayerPos;
			this->_requestedSeed = newSeed;
			this->_requestedScale = newScale;
			this->_requestedMap = newMap;
			this->_hasRequest = true;
		}
		_cv.notify_one();
	}

	const std::vector<uint8_t> &M_Minimap::GetLatestBuffer()
	{
		std::lock_guard<std::mutex> lock(_processMutex);
		return _frontBuffer;
	}

	void M_Minimap::WorkerLoop()
	{
		while (this->_running)
		{
			std::unique_lock lock(this->_cvMutex);
			_cv.wait(lock, [this] { return this->_hasRequest || !this->_running; });

			if (_running == false)
				break;

			MGL::Vectors::Vector2<int> requestPos = {static_cast<int>(this->_requestedPos[0]), static_cast<int>(this->_requestedPos[2])};
			uint32_t requestSeed = this->_requestedSeed;
			float requestScale = _requestedScale;
			uint8_t map = _requestedMap;
			lock.unlock();

			this->Generate(this->_backBuffer, requestPos, requestSeed, requestScale, map);

			{
				std::lock_guard FrontLock(this->_processMutex);
				std::swap(this->_frontBuffer, this->_backBuffer);
			}
		}
	}

	void M_Minimap::Generate(std::vector<uint8_t> &targetBuffer, MGL::Vectors::Vector2<int> playerPos, uint32_t seed, float scale, uint8_t map)
	{
		if (map == 0)
        	Game::Generation::Perlins::GenerateBiomeImage(targetBuffer, playerPos, seed, 400, scale);
		else
			Game::Generation::Perlins::GeneratePerlinImage(targetBuffer, playerPos, seed, 400, scale, map);
	}
} // namespace Vox::Game::Scenes::World::Interfaces