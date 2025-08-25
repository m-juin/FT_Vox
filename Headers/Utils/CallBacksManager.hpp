#ifndef __CALLBACKSMANAGER_HPP__
#define __CALLBACKSMANAGER_HPP__

#include <functional>
#include <memory>
#include <vector>

namespace Vox::Utils
{
	template <typename... Params> class CallBacksManager
	{
		public:
			template <typename F>
			inline std::shared_ptr<std::function<void(Params...)>> AddCallBack(F &&callBack)
			{
				auto cb = std::make_shared<std::function<void(Params...)>>(std::forward<F>(callBack));
				_callbacks.push_back(cb);
				return cb;
			}

			void RemoveCallback(std::shared_ptr<std::function<void(Params...)>> ptr)
			{
				_callbacks.erase(
					std::remove_if(_callbacks.begin(), _callbacks.end(), [&ptr](const auto &cb) { return cb == ptr; }),
					_callbacks.end());
			}

            void Notify(Params... params)
            {
                for (auto cb : this->_callbacks)
                    if(*cb)
                        (*cb)(std::forward<Params>(params)...);
                    
            }

		private:
			std::vector<std::shared_ptr<std::function<void(Params...)>>> _callbacks;
	};
} // namespace Vox::Utils

#endif // __CALLBACKSMANAGER_HPP__