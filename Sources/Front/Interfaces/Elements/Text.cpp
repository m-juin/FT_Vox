#include "Front/Interfaces/Elements/Text.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/SyncObjects.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/StaticGUIPipeline.hpp"

#include "Game/GameManager.hpp"

namespace Vox::Front::Interfaces::Elements
{
	Text::Text(const Vox_Text_Constructor &st) : AElement(st.pos, st.size)
	{
		this->_textColor = st.color;
		this->_textContent = st.content;
		this->_scale = st.scale;
		this->_letterSpacing = st.letterSpace;

		ResetVertex();
	}

	Text::Text(Vector2 pos, Vector2 size, Color color, std::string content, float scale) : AElement(pos, size)
	{
		this->_textColor = color;
		this->_textContent = content;
		this->_scale = scale;

		ResetVertex();
	}

	Text::~Text()
	{
		CleanBuffer(2);
	}

	void Text::ResetVertex()
	{
		if (this->_textContent.size() == 0)
			return;
		float x = this->_pos[0];
		float y = this->_pos[1];

		auto &font =
			Game::GameManager::GetInstance().GetSceneManager().GetCurrentScene().GetTextureManager()->GetFont();

		const Vector2 screenSize(Rendering::SwapChain::GetInstance().GetExtent().width,
								 Rendering::SwapChain::GetInstance().GetExtent().height);

		auto imgType = E_ImageType::Font;

		std::vector<uint32_t> index;
		_vertex.clear();

		this->_vertex.reserve(this->_textContent.size() * 4);
		index.reserve(this->_textContent.size() * 6);

		size_t curIndex = 0;

		float textHeight = (font.GetAscent() - font.GetDescent()) * this->_scale;
		float baseline = y + (textHeight / 2.0f) + (font.GetDescent() * this->_scale);


		for (auto letter : this->_textContent)
		{
			auto chr = font[letter];

			float x0 = x + (chr.bearingX * _scale);
			float x1 = x0 + (chr.width * _scale);

			float y1 = baseline - (chr.bearingY * _scale);
			float y0 = y1 + (chr.height * _scale);

			auto letterBound = font.GetCharBounds(letter);

			float u0 = letterBound.topLeft[0];
			float v0 = letterBound.topLeft[1];
			float u1 = letterBound.bottomRight[0];
			float v1 = letterBound.bottomRight[1];

			this->_vertex.push_back(
				Vertex(PointPixelToVulkan({x0, y0}, screenSize), {u0, v1}, this->_textColor, imgType));
			this->_vertex.push_back(
				Vertex(PointPixelToVulkan({x0, y1}, screenSize), {u0, v0}, this->_textColor, imgType));
			this->_vertex.push_back(
				Vertex(PointPixelToVulkan({x1, y0}, screenSize), {u1, v1}, this->_textColor, imgType));

			this->_vertex.push_back(
				Vertex(PointPixelToVulkan({x1, y1}, screenSize), {u1, v0}, this->_textColor, imgType));


			index.push_back(curIndex);
			index.push_back(curIndex + 1);
			index.push_back(curIndex + 2);

			index.push_back(curIndex + 2);
			index.push_back(curIndex + 3);
			index.push_back(curIndex + 1);

			curIndex += 4;
			x += ((chr.advance + _letterSpacing) * _scale);
		}

		_indexCount = index.size();
		if (B_Vertex == nullptr)
		{
			B_Vertex = new dbuffer(1, this->_vertex.size() * sizeof(Vertex),
								  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
			B_Index = new dbuffer(1, _indexCount * sizeof(uint32_t),
								 VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

			B_Vertex->Create(this->_vertex.data());
			B_Index->Create(index.data());
		}

		else
		{
			B_Vertex->Update(this->_vertex.data(), this->_vertex.size() * sizeof(Vertex));
			B_Index->Update(index.data(), _indexCount * sizeof(uint32_t));
		}
	}

	void Text::Draw()
	{
		if (this->_textContent.size() == 0)
			return;
		auto cmdBuffer =
			Rendering::CommandsPool::GetInstance().GetBuffer(Rendering::SyncObjects::GetInstance().GetCurrentFrame());
		VkDeviceSize offsets[] = {0};
		// vkCmdPushConstants(cmdBuffer, Front::Rendering::Pipelines::PipelinesManager::GetInstance().operator[]<Front::Rendering::Pipelines::StaticGUIPipeline>("StaticGUI")->GetLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0,
						//    sizeof(this->_uvMappingData), &this->_uvMappingData);
		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &this->B_Vertex->GetBuffer(0), offsets);
		vkCmdBindIndexBuffer(cmdBuffer, this->B_Index->GetBuffer(0), 0, VK_INDEX_TYPE_UINT32);
		vkCmdDrawIndexed(cmdBuffer, static_cast<uint32_t>(_indexCount), 1, 0, 0, 0);
	}

	void Text::SetPos(const Vector2 newPos)
	{
		if (this->_pos == newPos)
			return;
		this->_pos = newPos;
		this->ResetVertex();
	}

	void Text::SetSize(const Vector2 newSize)
	{
		if (this->_size == newSize)
			return;
		this->_size = newSize;
		this->ResetVertex();
	}
	
	void Text::SetColor(const Color &newColor)
	{
		if (this->_textColor == newColor) return ;
		this->_textColor = newColor;
		if (this->B_Vertex == VK_NULL_HANDLE) return ;
		for (auto &vert : this->_vertex)
			vert.texColor = this->_textColor;
		this->B_Vertex->Update(this->_vertex.data(), this->_vertex.size() * sizeof(Vertex));
	}
	
	void Text::SetContent(const std::string &newContent)
	{
		if (newContent == this->_textContent) return ;
		this->_textContent = newContent;
		this->ResetVertex();
	}

	Vector2 Text::GetTextSize(const std::string &content, const float &scale, const float letterSpace)
	{
		Vector2 size;
		auto &font =
			Game::GameManager::GetInstance().GetSceneManager().GetCurrentScene().GetTextureManager()->GetFont();

		size[0] = 0;
		size[1] = (font.GetAscent() - font.GetDescent()) * scale;

		for (auto letter : content)
		{
			auto chr = font[letter];
			size[0] += ((chr.advance + letterSpace) * scale);
		}

		return size;
	}
	
	Vector2 Text::GetTextSize()
	{
		Vector2 size;
		auto &font =
			Game::GameManager::GetInstance().GetSceneManager().GetCurrentScene().GetTextureManager()->GetFont();

		size[0] = 0;
		size[1] = (font.GetAscent() - font.GetDescent()) * this->_scale;

		for (auto letter : this->_textContent)
		{
			auto chr = font[letter];
			size[0] += ((chr.advance + _letterSpacing) * this->_scale);
		}

		return size;
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