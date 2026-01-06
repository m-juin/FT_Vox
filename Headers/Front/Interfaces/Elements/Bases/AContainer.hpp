#ifndef __ACONTAINER_HPP__
#define __ACONTAINER_HPP__

#include "AClickable.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "MathGraphicalLib/Vectors/Defines.hpp"

namespace Vox::Front::Interfaces::Elements::Bases
{
	struct ContainerElement
	{
			std::string key;
			std::unique_ptr<AElement> elem;
			size_t renderIndex;
			bool isVisible;

			ContainerElement() = delete;

			ContainerElement(std::string key_, std::unique_ptr<AElement> elem_, size_t renderIndex_ = 0, bool isVisible_ = true)
				: key(std::move(key_)), elem(std::move(elem_)), renderIndex(renderIndex_), isVisible(isVisible_)
			{
			}

			~ContainerElement() = default;

			ContainerElement(const ContainerElement &) = delete;
			ContainerElement &operator=(const ContainerElement &) = delete;

			ContainerElement(ContainerElement &&) noexcept = default;
			ContainerElement &operator=(ContainerElement &&) noexcept = default;
	};

	class AContainer : public virtual AClickable
	{
		public:
			AContainer() {}

			virtual ~AContainer() {};

			void ResetVertex() override {};
			ContainerElement &AddElement(const std::string &key, std::unique_ptr<AElement> elem, size_t renderIndex = 0, bool isVisible = true);

			bool RemoveElement(const std::string &key);

			template <typename T> T *GetElement(const std::string &key)
			{
				return dynamic_cast<T *>(this->GetContainerElement(key).elem.get());
			}

			ContainerElement &GetContainerElement(const std::string &key);

			virtual void Draw() override;
			virtual void OnClick(const int &button, const int &action) override;
			bool IsHover(const Vector2Float &mousePos) override;

		protected:
			Vox::Utils::CallBacksManager<AElement&> onElementAdd;
			Vox::Utils::CallBacksManager<> onElementRemove;

			std::vector<ContainerElement> _content;

			inline std::vector<ContainerElement>::iterator FindElement(const std::string &key)
			{
				return std::find_if(this->_content.begin(), this->_content.end(),
									[key](ContainerElement &elem) { return elem.key == key; });
			}
	};
} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __ACONTAINER_HPP__
