#include "Game/ThreadManager.hpp"

#include "Game/Scenes/World/Generation/SplinesManager.hpp"

#include "Game/GameManager.hpp"

namespace Vox::Game
{
	ThreadManager::ThreadManager(size_t maxThread) : _maxThread(maxThread) {}

	ThreadManager::~ThreadManager()
	{
		CleanPool();
	}

	void ThreadManager::EnQueue(
		std::function<void(std::unordered_map<std::string, std::pair<const Spline::Spline, float>>)> newTask)
	{
		{
			std::unique_lock lock(this->_queueMutex);
			this->_tasks.emplace(std::move(newTask));
		}
		_cv.notify_one();
	}

	void ThreadManager::BuildPool(const Game::Generation::SplinesManager &sManager)
	{
		if (this->_pool.empty() == false)
		{
			throw std::runtime_error("Trying to rebuild threadPool while previous one wasn't cleaned.");
		}
		{
			// Assure-toi que le flag stop est remis à false avant de démarrer les threads
			std::scoped_lock lock(this->_queueMutex);
			this->_stop = false;
		}
		for (size_t i = 0; i < this->_maxThread; i++)
		{
			std::unordered_map<std::string, std::pair<const Spline::Spline, float>> copy = sManager.GetSplinesCopy();
			this->_pool.emplace_back(
				[this, copy]
				{
					this->print("[DEBUG] Thread ", std::this_thread::get_id(), " launched.");
					while (1)
					{
						std::function<void(std::unordered_map<std::string, std::pair<const Spline::Spline, float>>)>
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
						task(copy);
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

		std::queue<std::function<void(std::unordered_map<std::string, std::pair<const Spline::Spline, float>>)>> empty;
		std::swap(this->_tasks, empty);
		for (std::thread &thread : this->_pool)
			thread.join();
		this->_pool.clear();
	}

	void ThreadManager::RecreatePool()
	{
		this->CleanPool();
		this->BuildPool(Game::GameManager::GetInstance().GetSplineManager());
	}
} // namespace Vox::Game