#ifndef __ACONTAINER_HPP__
#define __ACONTAINER_HPP__

#include "AElement.hpp"
#include <unordered_map>
#include <vector>

#include <algorithm>
#include <iostream>

namespace Vox::Front::Interfaces::BaseClass
{
	class AContainer : public AElement
	{
		private:
			std::vector<ContainerElement> _content;

		public:
			AContainer() {};
			virtual ~AContainer()
			{
				this->_content.clear();
			}
			void AddElement(const std::string &key, AElement *elem, size_t renderIndex = 0)
			{
				if (this->FindElement(key) != this->_content.end())
					throw std::runtime_error("Trying to add an already existing element.");
				this->_content.push_back(ContainerElement(key, elem, renderIndex));

				if (_content.size() > 1 && _content.back().renderIndex < _content[_content.size() - 2].renderIndex)
				{
					std::sort(_content.begin(), _content.end(), [](ContainerElement &elem1, ContainerElement &elem2)
							  { return elem1.renderIndex < elem2.renderIndex; });
				}
			};

			bool RemoveElement(const std::string &key)
			{
				auto newEnd = std::remove_if(_content.begin(), _content.end(),
											 [key](const ContainerElement &elem) { return elem.key == key; });
				bool found = newEnd != _content.end();
				_content.erase(newEnd, _content.end());
				return found;
			}

			template <typename T> T *GetElement(const std::string &key)
			{
				return dynamic_cast<T *>(this->GetContainerElement(key));
			}

			ContainerElement &GetContainerElement(const std::string &key)
			{
				auto it = this->FindElement(key);
				if (it == this->_content.end())
					throw std::runtime_error("Element \"" + key + "\" not found.");
				return *it;
			}

			void Draw()
			{
				for (ContainerElement &cElem : this->_content)
					if (cElem.elem != nullptr)
						cElem.elem->Draw();
			}

		private:
			std::vector<ContainerElement>::iterator FindElement(const std::string &key)
			{
				return std::find_if(this->_content.begin(), this->_content.end(),
									[key](ContainerElement &elem) { elem.key == key; });
			}
	};

	struct ContainerElement
	{
			std::string key;
			AElement *elem;
			size_t renderIndex;

			ContainerElement() = delete;
			ContainerElement(std::string key_, AElement *elem_, size_t renderIndex_ = 0)
				: key(key_), elem(elem_), renderIndex(renderIndex_) {};
			~ContainerElement()
			{
				if (elem)
					delete elem;
			};
	};
} // namespace Vox::Front::Interfaces::BaseClass

#endif // __ACONTAINER_HPP__