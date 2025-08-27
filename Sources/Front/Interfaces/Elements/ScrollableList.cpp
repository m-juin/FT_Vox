#include "Front/Interfaces/Elements/ScrollableList.hpp"

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
			[](const double &xOff, const double &yOff)
			{
				(void)xOff;
				std::cout << yOff << std::endl;
			});
	}

	void ScrollableList::Draw()
	{
		Vector2 validHeight = {this->_pos[1] + scrollOffset, this->_pos[1] + scrollOffset + this->_size[1]};
		for (auto &elem : this->_content)
		{
			const float &elemPos = elem.elem->GetPos()[1];
			const float &elemSize = elem.elem->GetSize()[1];
			if (elemPos + elemSize >= validHeight[0] && elemPos + elemSize <= validHeight[1])
				elem.elem->Draw();
		}
	}

	void ScrollableList::RebuildList()
	{
		size_t curOffset = 0;
		for (auto &elem : this->_content)
		{
			if (elem.elem == nullptr)
				continue;
			elem.elem->SetSize({this->_size[0] - 10, elem.elem->GetSize()[1]});

			elem.elem->SetPos({this->_pos[0] + 5, this->_pos[1] + curOffset + 5});
			curOffset += elem.elem->GetSize()[1] + 5;
		}
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