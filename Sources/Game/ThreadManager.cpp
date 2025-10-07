#include "Game/ThreadManager.hpp"

#include "Game/Scenes/World/Generation/SplinesManager.hpp"

#include "Game/GameManager.hpp"

#include "Front/Utils/TexturesAtlas.hpp"
#include "Front/Scenes/TexturesManager.hpp"

namespace Vox::Game
{
	ThreadManager::ThreadManager(size_t maxThread) : _maxThread(maxThread) {}

	ThreadManager::~ThreadManager()
	{
		CleanPool();
	}

	void ThreadManager::EnQueue(
		std::function<void(std::unordered_map<std::string, std::pair<const Spline::Spline, float>>, std::vector<Game::Utils::Textures::TextureInfo>,  std::vector<Game::Utils::Textures::TextureInfo>)> newTask)
	{
		{
			std::unique_lock lock(this->_queueMutex);
			this->_tasks.emplace(std::move(newTask));
		}
		_cv.notify_one();
	}

	void ThreadManager::BuildPool(const Game::Generation::SplinesManager &sManager, const Front::Scenes::TexturesManager &tManager)
	{
		if (this->_pool.empty() == false)
		{
			throw std::runtime_error("Trying to rebuild threadPool while previous one wasn't cleaned.");
		}
		{
			std::scoped_lock lock(this->_queueMutex);
			this->_stop = false;
		}
		for (size_t i = 0; i < this->_maxThread; i++)
		{
			std::unordered_map<std::string, std::pair<const Spline::Spline, float>> copy = sManager.GetSplinesCopy();
			auto textInfo = tManager.operator[]("A_Blocks").GetTextureInfo();
			auto transparent = tManager.operator[]("A_Blocks_Transparent").GetTextureInfo();
			this->_pool.emplace_back(
				[this, copy, textInfo, transparent]
				{
					this->print("[DEBUG] Thread ", std::this_thread::get_id(), " launched.");
					while (1)
					{
						std::function<void(std::unordered_map<std::string, std::pair<const Spline::Spline, float>>, std::vector<Game::Utils::Textures::TextureInfo>, std::vector<Game::Utils::Textures::TextureInfo>)>
							task;
						{
							std::unique_lock<std::mutex> lock(this->_queueMutex);

							_cv.wait(lock, [this] { return !_tasks.empty() || this->_stop; });
							if (_stop)
								return;
							task = std::move(this->_tasks.front());
							_tasks.pop();
							lock.unlock();
						}
						task(copy, textInfo, transparent);
					}
				});
		}
	}

	void ThreadManager::CleanPool()
	{
		{
			std::unique_lock lock(this->_queueMutex);
			_stop = true;
		}
		_cv.notify_all();

		std::queue<std::function<void(std::unordered_map<std::string, std::pair<const Spline::Spline, float>>, std::vector<Game::Utils::Textures::TextureInfo>, std::vector<Game::Utils::Textures::TextureInfo>)>> empty;
		std::swap(this->_tasks, empty);
		for (std::thread &thread : this->_pool)
			thread.join();
		this->_pool.clear();
	}

	void ThreadManager::RecreatePool()
	{
		this->CleanPool();
		this->BuildPool(Game::GameManager::GetInstance().GetSplineManager(), Game::GameManager::GetInstance().GetTexturesManager());
	}
} // namespace Vox::Game