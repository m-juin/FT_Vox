#include "Front/Interfaces/Elements/ScrollableList.hpp"

#include <algorithm>

namespace Vox::Front::Interfaces::Elements
{
	ScrollableList::ScrollableList(Vox_ScrollableList_Constructor &st)
		: AElement(st.pos, st.size), IContainer(), AScrollable()
	{
		for (auto &elem : st.content)
			this->AddElement(std::move(elem), 1);
		this->onElementAdd.AddCallBack(
			[this](AElement &added)
			{
				(void)added;
				this->RebuildList();
			});

		this->onElementRemove.AddCallBack([this]() { this->RebuildList(); });

		this->RebuildList();

		this->onScrollCallBacks.AddCallBack(
			[this](const double &xOff, const double &yOff)
			{
				(void)xOff;

				constexpr float scrollStep = -20.f;
				this->_scrollOffset += yOff * scrollStep;

				// la limite basse = taille totale du contenu - taille visible
				float maxOffset = std::max(0.f, _fullSize - _size[1]);
				_scrollOffset = std::clamp(_scrollOffset, 0.f, maxOffset);

				this->RebuildList();
			});
	}
	
	void ScrollableList::OnClick(const int &button, const int &action)
	{
		IContainer::OnClick(button, action);
	}
	
	bool ScrollableList::IsHover(const Vector2& mousePos)
	{
		if (this->_enabled == false)
			return false;

		if ((mousePos[0] >= this->_pos[0] && mousePos[0] <= this->_pos[0] + this->_size[0]) &&
			(mousePos[1] >= this->_pos[1] && mousePos[1] <= this->_pos[1] + this->_size[1]))
		{
			currentlyHovered = true;
			// std::cout << "hover\n";
			this->OnHover();
		}
		else
		{
			currentlyHovered = false;
			this->OnHoverLeave();
		}
		return IContainer::IsHover(mousePos);
	}

	// Draw() — plus d'ajout de _scrollOffset ici (c'est déjà dans les positions)
	void ScrollableList::Draw()
	{
		for (auto &elem : this->_content)
		{
			if (elem.elem->GetPos()[1] < this->_pos[1] ||
				elem.elem->GetPos()[1] + elem.elem->GetSize()[1] > this->_pos[1] + this->_size[1])
				continue;
			elem.elem->Draw();
		}
	}

	void ScrollableList::RebuildList()
	{
		float curOffset = 0.f; // commence à 0 = haut du contenu

		for (auto &elem : this->_content)
		{
			if (!elem.elem)
				continue;

			elem.elem->SetSize({this->_size[0] - 10.f, elem.elem->GetSize()[1]});

			// position = base + offset contenu - scroll
			elem.elem->SetPos({this->_pos[0] + 5.f, this->_pos[1] + curOffset - this->_scrollOffset});

			curOffset += elem.elem->GetSize()[1] + 5.f;
		}

		// maintenant _fullSize = hauteur totale du contenu
		this->_fullSize = curOffset;
	}

	void ScrollableList::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		RebuildList();
	}

	void ScrollableList::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->RebuildList();
	}

} // namespace Vox::Front::Interfaces::Elements