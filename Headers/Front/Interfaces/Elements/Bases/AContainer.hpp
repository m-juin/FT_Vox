#ifndef __ACONTAINER_HPP__
#define __ACONTAINER_HPP__

#include "AClickable.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Vox::Front::Interfaces::Elements::Bases
{
	struct ContainerElement
	{
			std::string key;
			std::unique_ptr<AElement> elem;
			size_t renderIndex;

			ContainerElement() = delete;

			ContainerElement(std::string key_, std::unique_ptr<AElement> elem_, size_t renderIndex_ = 0)
				: key(std::move(key_)), elem(std::move(elem_)), renderIndex(renderIndex_)
			{
			}

			~ContainerElement() = default;

			ContainerElement(const ContainerElement &) = delete;
			ContainerElement &operator=(const ContainerElement &) = delete;

			ContainerElement(ContainerElement &&) noexcept = default;
			ContainerElement &operator=(ContainerElement &&) noexcept = default;
	};

	class AContainer : public AClickable
	{
		public:
			AContainer(Vector2 pos = {0, 0}, Vector2 size = {100, 50}) : AClickable(pos, size) {}

			virtual ~AContainer() {};

			void ResetVertex() override {};
			void AddElement(const std::string &key, std::unique_ptr<AElement> elem, size_t renderIndex = 0);

			bool RemoveElement(const std::string &key);

			template <typename T> T *GetElement(const std::string &key)
			{
				return dynamic_cast<T *>(this->GetContainerElement(key).elem.get());
			}

			ContainerElement &GetContainerElement(const std::string &key);

			virtual void SetPos(const Vector2 newPos) override = 0;
			virtual void SetSize(const Vector2 newSize) override = 0;

			void Draw() override;

			void OnHover() override;
			void OnClick() override;
			void OnHoverLeave() override;
			bool IsHover(const Vector2 &mousePos) override;

		protected:
			std::vector<ContainerElement> _content;

			inline std::vector<ContainerElement>::iterator FindElement(const std::string &key)
			{
				return std::find_if(this->_content.begin(), this->_content.end(),
									[key](ContainerElement &elem) { return elem.key == key; });
			}
	};
} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __ACONTAINER_HPP__
