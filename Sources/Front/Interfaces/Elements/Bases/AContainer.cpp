#include "Front/Interfaces/Elements/Bases/AContainer.hpp"

namespace Vox::Front::Interfaces::Elements::Bases
{
	void AContainer::AddElement(const std::string &key, std::unique_ptr<AElement> elem, size_t renderIndex)
	{
		if (this->FindElement(key) != this->_content.end())
			throw std::runtime_error("Trying to add an already existing element.");

		_content.emplace_back(key, std::move(elem), renderIndex);

		if (_content.size() > 1 && _content.back().renderIndex < _content[_content.size() - 2].renderIndex)
		{
			std::sort(_content.begin(), _content.end(), [](ContainerElement &elem1, ContainerElement &elem2)
					  { return elem1.renderIndex < elem2.renderIndex; });
		}
		this->onElementAdd.Notify(*elem);
	}

	bool AContainer::RemoveElement(const std::string &key)
	{
		auto it = FindElement(key);

		if (it == _content.end())
			return false;

		// Suppression de l'élément
		_content.erase(it);

		onElementRemove.Notify();
		return true;
	}

	ContainerElement &AContainer::GetContainerElement(const std::string &key)
	{
		auto it = this->FindElement(key);
		if (it == this->_content.end())
			throw std::runtime_error("Element \"" + key + "\" not found.");
		return *it;
	}

	void AContainer::Draw()
	{
		for (ContainerElement &cElem : this->_content)
			if (cElem.elem != nullptr)
				cElem.elem->Draw();
	}

	void AContainer::OnClick(const int &button, const int &action)
	{
		for (auto &elem : this->_content)
		{
			if (auto child = dynamic_cast<AClickable *>(elem.elem.get()))
			{
				if (child->currentlyHovered)
					child->OnClick(button, action);
			}
		}
	}

	bool AContainer::IsHover(const Vector2 &mousePos)
	{
		// std::cout << "AContainer::IsHover\n";
		bool hovered = false;
		for (auto &elem : this->_content)
		{
			if (auto child = dynamic_cast<AClickable *>(elem.elem.get()))
			{
				if (child->IsHover(mousePos))
					hovered = true;
			}
		}
		this->currentlyHovered = AClickable::IsHover(mousePos);
		return hovered;
	}

} // namespace Vox::Front::Interfaces::Elements::Bases