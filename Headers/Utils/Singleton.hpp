#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#include <iostream>
#include <memory>
#include <stdexcept>

namespace Vox::Utils
{
	template <typename T> class Singleton
	{
		public:
			template <typename... Args> static void Init(Args &&...args)
			{
				if (!instance)
				{
					instance = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
				}
				else
				{
					throw std::runtime_error("Already initialized instance!");
				}
			}

			static T &GetInstance()
			{
				if (!instance)
				{
					std::string error =
						"Instance of " + std::string(typeid(T).name()) + " not initialized! Call Init() before.";
					throw std::runtime_error(error);
				}

				return *instance;
			}

			static bool IsInit()
			{
				return !(instance == nullptr);
			}

			static void Clean()
			{
				if (!instance)
					return;
				instance.reset();
			}

			Singleton(const Singleton &) = delete;
			Singleton &operator=(const Singleton &) = delete;

		protected:
			Singleton() = default;
			virtual ~Singleton() = default;

		private:
			static std::unique_ptr<T> instance;
	};

	template <typename T> std::unique_ptr<T> Singleton<T>::instance = nullptr;
} // namespace Vox::Utils
#endif // __SINGLETON_HPP__