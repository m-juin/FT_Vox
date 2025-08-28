#include "Front/Interfaces/Elements/Bases/IContainer.hpp"

namespace Vox::Front::Interfaces::Elements::Bases
{
	void IContainer::AddElement(std::unique_ptr<AElement> elem, size_t renderIndex)
	{
		_content.emplace_back(std::move(elem), renderIndex);

		if (_content.size() > 1 && _content.back().renderIndex < _content[_content.size() - 2].renderIndex)
		{
			std::sort(_content.begin(), _content.end(),
					  [](IndexedContainerElement &elem1, IndexedContainerElement &elem2)
					  { return elem1.renderIndex < elem2.renderIndex; });
		}
		this->onElementAdd.Notify(*elem);
	}

	bool IContainer::RemoveElement(size_t index)
	{
		if (index >= _content.size())
			return false;
		// Suppression de l'élément
		_content.erase(_content.begin() + index);
		// Notification des callbacks avant suppression
		onElementRemove.Notify();

		return true;
	}

	void IContainer::Draw()
	{
		for (IndexedContainerElement &cElem : this->_content)
			if (cElem.elem != nullptr)
				cElem.elem->Draw();
	}

	void IContainer::OnHover() {}

	void IContainer::OnClick(const int &button, const int &action)
	{
		for (auto &elem : this->_content)
		{
			if (auto child = dynamic_cast<AClickable *>(elem.elem.get()))
			{
				// if (child->currentlyHovered)
				child->OnClick(button, action);
			}
		}
	}

	void IContainer::OnHoverLeave() {}

	bool IContainer::IsHover(const Vector2 &mousePos, bool override)
	{
		bool hovered = false;
		for (auto elem = this->_content.rbegin(); elem != this->_content.rend(); ++elem)
		{
			if (auto child = dynamic_cast<AClickable *>(elem->elem.get()))
			{
				if (child->IsHover(mousePos, override))
					hovered = true;
			}
		}
		this->currentlyHovered = AClickable::IsHover(mousePos, override);
		return hovered;
	}

} // namespace Vox::Front::Interfaces::Elements::Bases