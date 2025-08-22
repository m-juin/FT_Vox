#include "Front/Interfaces/Elements/Text.hpp"

#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include "Game/GameManager.hpp"

#include "Front/Interfaces/Utils/Maths.hpp"

namespace Vox::Front::Interfaces::Elements
{
	Text::Text(const Vox_Text_Constructor &st) : AElement(st.pos, st.size)
	{
		this->_textColor = st.color;
		this->_textContent = st.content;
		this->_scale = st.scale;

        ResetVertex();
	}

	Text::Text(Bases::Vector2 pos, Bases::Vector2 size, Color color, std::string content, float scale)
		: AElement(pos, size)
	{
		this->_textColor = color;
		this->_textContent = content;
		this->_scale = scale;

        ResetVertex();
	}

	Text::~Text() {}

	void Text::ResetVertex()
	{
        this->CleanBuffer(0);
		if (this->_textContent.size() == 0)
			return;
		float x = this->_pos[0];
		float y = this->_pos[1];

		auto &font =
			Game::GameManager::GetInstance().GetSceneManager().GetCurrentScene().GetTextureManager()->GetFont();

		const Bases::Vector2 screenSize(Rendering::SwapChain::GetInstance().GetExtent().width,
										Rendering::SwapChain::GetInstance().GetExtent().height);

		auto imgType = E_ImageType::Font;

		std::vector<uint32_t> index;

		this->_vertex.reserve(this->_textContent.size() * 4);
		index.reserve(this->_textContent.size() * 6);

		size_t curIndex = 0;

		for (auto letter : this->_textContent)
		{
			auto chr = font[letter];

			float textHeight = (font.GetAscent() - font.GetDescent()) * this->_scale;
			float baseline = y + (textHeight / 2.0f) + (font.GetDescent() * this->_scale);

			size_t x0 = x + (chr.bearingX * _scale);
			size_t x1 = x0 + (chr.width * _scale);

			size_t y1 = baseline - (chr.bearingY * _scale);
			size_t y0 = y1 + (chr.height * _scale);

			auto letterBound = font.GetCharBounds(letter);

			float u0 = letterBound.topLeft[0];
			float v0 = letterBound.topLeft[1];
			float u1 = letterBound.bottomRight[0];
			float v1 = letterBound.bottomRight[1];

			this->_vertex.push_back(
				vert(Utils::Maths::PointPixelToVulkan({x0, y0}, screenSize), {u0, v1}, this->_textColor, imgType));
			this->_vertex.push_back(
				vert(Utils::Maths::PointPixelToVulkan({x0, y1}, screenSize), {u0, v0}, this->_textColor, imgType));
			this->_vertex.push_back(
				vert(Utils::Maths::PointPixelToVulkan({x1, y0}, screenSize), {u1, v1}, this->_textColor, imgType));

			// this->_vertex.push_back(vert(Utils::Maths::PointPixelToVulkan({x1, y0}, screenSize), {u1, v1},
			// this->_textColor, imgType));
			this->_vertex.push_back(
				vert(Utils::Maths::PointPixelToVulkan({x1, y1}, screenSize), {u1, v0}, this->_textColor, imgType));
			// this->_vertex.push_back(vert(Utils::Maths::PointPixelToVulkan({x0, y1}, screenSize), {u0, v0},
			// this->_textColor, imgType));

			index.push_back(curIndex);
			index.push_back(curIndex + 1);
			index.push_back(curIndex + 2);

			index.push_back(curIndex + 2);
			index.push_back(curIndex + 3);
			index.push_back(curIndex + 1);

			curIndex += 4;
            x += (chr.advance * _scale);
		}

		B_Vertex = new buffer(1, this->_vertex.size() * sizeof(vert),
							  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
		B_Index = new buffer(1, index.size() * sizeof(uint32_t),
							 VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

		B_Vertex->Create(this->_vertex.data());
		B_Index->Create(index.data());

        for (auto vertex : this->_vertex)
            std::cout << vertex.position << std::endl;
	}

	void Text::Draw()
	{
		if (this->B_Vertex == nullptr)
			return;
		auto cmdBuffer =
			Rendering::CommandsPool::GetInstance().GetBuffer(Rendering::SyncObjects::GetInstance().GetCurrentFrame());
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &this->B_Vertex->GetBuffer(0), offsets);
		vkCmdBindIndexBuffer(cmdBuffer, this->B_Index->GetBuffer(0), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(cmdBuffer, this->_vertex.size() * 6, 1, 0, 0, 0);
	}

	void Text::SetPos(const Bases::Vector2 newPos)
	{
		if (this->_pos == newPos)
			return;
		this->_pos = newPos;
	}

	void Text::SetSize(const Bases::Vector2 newSize)
	{
		if (this->_size == newSize)
			return;
		this->_size = newSize;
	}

	void Text::CleanBuffer(size_t mode)
	{
		if ((mode == 0 || mode == 2) && this->B_Vertex)
		{
			delete this->B_Vertex;
			this->B_Vertex = nullptr;
		}
		if (mode >= 1 && this->B_Index)
		{
			delete this->B_Index;
			this->B_Index = nullptr;
		}
	}
} // namespace Vox::Front::Interfaces::Elements