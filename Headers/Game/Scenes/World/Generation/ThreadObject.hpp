#ifndef __THREADOBJECT_HPP__
#define __THREADOBJECT_HPP__

#include "Game/Scenes/World/Chuncks/E_GenerationState.hpp"

#include <mutex>

namespace Vox::Game::Generation::Threads
{
	class ThreadObject
	{
		public:
			ThreadObject() {};
			virtual ~ThreadObject() {};

			inline void ChangeGenerationState(const Game::Generation::E_GenerationState &newState)
			{
				std::unique_lock<std::mutex> lock(this->_mutex);
				// if (this->_generationState == Generation::GenerationState::Cancelled) return ;
				this->_currentState = newState;
				lock.unlock();
			}

			inline Game::Generation::E_GenerationState GetGenerationState()
			{
				std::unique_lock<std::mutex> lock(this->_mutex);
				Game::Generation::E_GenerationState state = this->_currentState;
				lock.unlock();
				return state;
			}

		private:
            std::mutex _mutex;
        protected:
            Game::Generation::E_GenerationState _currentState;
			/* private */
	};
} // namespace Vox::Game::Generation::Threads

#endif // __THREADOBJECT_HPP__