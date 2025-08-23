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
	}

	bool AContainer::RemoveElement(const std::string &key)
	{
		auto newEnd = std::remove_if(_content.begin(), _content.end(),
									 [key](const ContainerElement &elem) { return elem.key == key; });
		bool found = newEnd != _content.end();
		_content.erase(newEnd, _content.end());
		return found;
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

	void AContainer::OnHover()
	{
		
	}

	void AContainer::OnClick()
	{
		
	}

	void AContainer::OnHoverLeave()
	{
		
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
		return hovered;
	}

} // namespace Vox::Front::Interfaces::Elements::Bases