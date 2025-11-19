#include "Front/Interfaces/Elements/Bases/AContainer.hpp"

namespace Vox::Front::Interfaces::Elements::Bases
{
	ContainerElement &AContainer::AddElement(const std::string &key, std::unique_ptr<AElement> elem, size_t renderIndex, bool isVisible)
	{
		if (this->FindElement(key) != this->_content.end())
			throw std::runtime_error("Trying to add an already existing element.");

		auto& newElem = *elem;
		auto &it = _content.emplace_back(key, std::move(elem), renderIndex, isVisible);

		if (_content.size() > 1 && _content.back().renderIndex < _content[_content.size() - 2].renderIndex)
		{
			std::sort(_content.begin(), _content.end(), [](ContainerElement &elem1, ContainerElement &elem2)
					  { return elem1.renderIndex < elem2.renderIndex; });
		}
		this->onElementAdd.Notify(newElem);
		return it;
	}

	bool AContainer::RemoveElement(const std::string &key)
	{
		auto it = FindElement(key);

		if (it == _content.end())
			return false;

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
		if (this->_enabled == false) return ;
		for (auto &elem : this->_content)
		{
			if (auto child = dynamic_cast<AClickable *>(elem.elem.get()))
			{
				child->OnClick(button, action);
			}
		}
	}

	bool AContainer::IsHover(const Vector2 &mousePos)
	{
		if (this->_enabled == false) return false;
		for (auto elem = this->_content.rbegin(); elem != this->_content.rend(); ++elem)
		{
			if (auto child = dynamic_cast<AClickable *>(elem->elem.get()))
			{
				child->IsHover(mousePos);
			}
		}
		return false;
	}

} // namespace Vox::Front::Interfaces::Elements::Bases