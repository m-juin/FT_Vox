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

				constexpr float scrollStep = -20.f; // ajuste à ton goût
				this->_scrollOffset += yOff * scrollStep;

				// calcule la borne max en float (évite le wrapping)
				// float maxOffset = std::max(0.f, static_cast<float>(this->_fullSize) - this->_size[1]);

				// clamp propre
				if (_scrollOffset < 0)
					_scrollOffset = 0;
				else if (_scrollOffset > this->_fullSize - this->_size[1])
					_scrollOffset = this->_fullSize - this->_size[1];
				std::cout << _scrollOffset << std::endl;
				// this->_scrollOffset = std::clamp((size_t)this->_scrollOffset, (size_t)0, (size_t)maxOffset);

				// on rebuild les positions (elles utilisent _scrollOffset)
				this->RebuildList();
			});
	}

	// Draw() — plus d'ajout de _scrollOffset ici (c'est déjà dans les positions)
	void ScrollableList::Draw()
	{
		for (auto &elem : this->_content)
		{
			if (elem.elem->GetPos()[1] < this->_pos[1] || elem.elem->GetPos()[1] + elem.elem->GetSize()[1] > this->_pos[1] + this->_size[1])
				continue;
			elem.elem->Draw();
		}
	}

	void ScrollableList::RebuildList()
	{
		float curOffset = this->_pos[1]; // float pour éviter conversions surprises

		for (auto &elem : this->_content)
		{
			if (elem.elem == nullptr)
				continue;

			elem.elem->SetSize({this->_size[0] - 10, elem.elem->GetSize()[1]});

			// position réelle = base + curOffset - scrollOffset
			elem.elem->SetPos({this->_pos[0] + 5, curOffset + this->_scrollOffset});

			curOffset += elem.elem->GetSize()[1] + 5;
		}

		// _fullSize = taille totale du contenu (sans tenir compte du scroll)
		this->_fullSize = static_cast<size_t>(curOffset);
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