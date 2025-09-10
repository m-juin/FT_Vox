#ifndef __THREADMANAGER_HPP__
#define __THREADMANAGER_HPP__

#include <queue>
#include <thread>

#include <functional>
#include <future>
#include <mutex>

#include <iostream>

namespace Spline
{
	class Spline;
} // namespace Spline

namespace Vox::Game
{
	namespace Generation {class SplinesManager;};
	class GameManager;
	class ThreadManager
	{
			friend class GameManager;

		public:
			template <typename... Args> static void print(Args &&...args)
			{
				std::lock_guard<std::mutex> lock(GetCoutMutex());
				(std::cout << ... << args) << std::endl;
			}
			static std::mutex &GetCoutMutex()
			{
				static std::mutex coutMutex;
				return coutMutex;
			}
			ThreadManager() = delete;
			ThreadManager(const ThreadManager &) = delete;
			ThreadManager(ThreadManager &) = delete;

			ThreadManager(size_t maxThread);
			~ThreadManager();

			void EnQueue(std::function<void(std::unordered_map<std::string, std::pair<const Spline::Spline, float>>)> newTask);

		private:
			void BuildPool(Game::Generation::SplinesManager &sManager);
			void CleanPool();

			const uint8_t _maxThread;
			std::mutex _queueMutex;
			std::condition_variable _cv;
			bool _stop = false;

			std::vector<std::thread> _pool;
			std::queue<std::function<void(std::unordered_map<std::string, std::pair<const Spline::Spline, float>>)>> _tasks;
			/* private */
	};
} // namespace Vox::Game

#endif // __THREADMANAGER_HPP__