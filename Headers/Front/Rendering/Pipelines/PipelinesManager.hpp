#ifndef __PIPELINESMANAGER_HPP__
#define __PIPELINESMANAGER_HPP__

#include <vulkan/vulkan.h>

#include "Utils/Singleton.hpp"

#include <unordered_map>

namespace Vox::Front::Rendering::Pipelines
{
	class APipeline;
	class PipelinesManager : public Vox::Utils::Singleton<PipelinesManager>
	{
		private:
			friend class Vox::Utils::Singleton<PipelinesManager>;
			VkRenderPass _renderPass;
			void CreateRenderPass();

			std::unordered_map<std::string, APipeline *> _pipelines;

			PipelinesManager();

		public:
			~PipelinesManager();

			void BindPipeline(const std::string &key);

			VkRenderPass &GetRenderPass()
			{
				return this->_renderPass;
			};

			template<typename T>
			T *operator[](std::string name);
	};
	
} // namespace Vox::Front::Rendering::Pipelines
#endif // __PIPELINESMANAGER_HPP__