#include "Game/ThreadManager.hpp"

#include "Game/Scenes/World/Generation/SplinesManager.hpp"

namespace Vox::Game
{
	ThreadManager::ThreadManager(size_t maxThread) : _maxThread(maxThread)
	{
	}

	ThreadManager::~ThreadManager()
	{
		CleanPool();
	}

	void ThreadManager::EnQueue(std::function<void(std::unordered_map<std::string, const Spline::Spline>)> newTask)
	{
		{
			std::unique_lock lock(this->_queueMutex);
			this->_tasks.emplace(std::move(newTask));
		}
		_cv.notify_one();
	}


	void ThreadManager::BuildPool(Game::Generation::SplinesManager &sManager)
	{
		if (this->_pool.empty() == false)
		{
			throw std::runtime_error("Trying to rebuild threadPool while previous one wasn't cleaned.");
		}
		for (size_t i = 0; i < this->_maxThread; i++)
		{
			std::unordered_map<std::string, const Spline::Spline> copy = sManager.GetSplinesCopy();
			this->_pool.emplace_back(
				[this, copy]
				{
                    this->print("[DEBUG] Thread ", std::this_thread::get_id(), " launched.");
					while (1)
					{
						std::function<void(std::unordered_map<std::string, const Spline::Spline>)> task;
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

		for (std::thread &thread : this->_pool)
			thread.join();
	}
} // namespace Vox::Game::ThreadManager