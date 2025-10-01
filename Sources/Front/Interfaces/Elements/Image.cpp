#include "Front/Interfaces/Elements/Image.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/StaticGUIPipeline.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include "Front/Interfaces/Utils/Maths.hpp"

#include "Game/GameManager.hpp"
#include "Game/Scenes/Menu/Sc_Menu.hpp"

namespace Vox::Front::Interfaces::Elements
{

	Image::Image(std::string atlas, std::string key, Vector2 pos, Vector2 size, Color colorMod, Vector2 textureRepeat)
		: Bases::AElement(pos, size), _atlas(atlas), _atlasKey(key), _colorMod(colorMod), _textureRepeat(textureRepeat)
	{
		this->ResetVertex();

		B_Vertices =
			new dbuffer(1, 4 * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
		B_Indices =
			new dbuffer(1, 6 * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

		B_Vertices->Create(&this->_vertex);
		// std::cout <<"creation2" << std::endl;
		uint16_t *indices = new uint16_t[6]{0, 1, 2, 2, 3, 0};

		B_Indices->Create(indices);

		delete[] indices;
	}

	Image::~Image()
	{
		if (B_Vertices)
			delete B_Vertices;
		if (B_Indices)
			delete B_Indices;
	}

	void Image::CleanBuffers(size_t mode)
	{
		if ((mode == 0 || mode == 2) && this->B_Vertices != nullptr)
		{
			delete this->B_Vertices;
			this->B_Vertices = nullptr;
		}
		if (mode >= 1 && this->B_Indices != nullptr)
		{
			delete this->B_Indices;
			this->B_Indices = nullptr;
		}
	}

	void Image::ResetVertex()
	{

		// this->CleanBuffers(0);

		const Vector2 screenSize(Rendering::SwapChain::GetInstance().GetExtent().width,
								 Rendering::SwapChain::GetInstance().GetExtent().height);

		this->_uvMappingData.textureID = 0;
		if (this->_atlas != "" && this->_atlasKey != "")
		{
			auto atlas = Game::GameManager::GetInstance().GetTexturesManager()[_atlas];
			auto uvData = atlas->GetTextureInfo(this->_atlasKey);

			this->_uvMappingData.uvMin[0] = uvData.uOffset;
			this->_uvMappingData.uvMin[1] = uvData.vOffset;
			this->_uvMappingData.uvMax[0] = uvData.uOffset + uvData.uSize;
			this->_uvMappingData.uvMax[1] = uvData.vOffset + uvData.vSize;
			this->_uvMappingData.atlasSize = atlas->_mainAtlas->GetSize();

			this->_vertex[0] = Vertex(PointPixelToVulkan(this->_pos, screenSize), {0.0f, 0.0f}, this->_colorMod);
			this->_vertex[1] = Vertex(PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1]}, screenSize),
									  {1.0f * _textureRepeat[0], 0.0f}, this->_colorMod);
			this->_vertex[2] =
				Vertex(PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1] + this->_size[1]}, screenSize),
					   {1.0f * _textureRepeat[0], 1.0f * _textureRepeat[1]}, this->_colorMod);
			this->_vertex[3] = Vertex(PointPixelToVulkan({this->_pos[0], this->_pos[1] + this->_size[1]}, screenSize),
									  {0.0f, 1.0f * _textureRepeat[1]}, this->_colorMod);
		}
		else
		{
			this->_uvMappingData.uvMin[0] = 0.0f;
			this->_uvMappingData.uvMin[1] = 0.0f;
			this->_uvMappingData.uvMax[0] = 1.0f;
			this->_uvMappingData.uvMax[1] = 1.0f;

			this->_vertex[0] =
				Vertex(PointPixelToVulkan(this->_pos, screenSize), {0.0f, 0.0f}, this->_colorMod, E_ImageType::Color);
			this->_vertex[1] = Vertex(PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1]}, screenSize),
									  {1.0f, 0.0f}, this->_colorMod, E_ImageType::Color);
			this->_vertex[2] =
				Vertex(PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1] + this->_size[1]}, screenSize),
					   {1.0f, 1.0f}, this->_colorMod, E_ImageType::Color);
			this->_vertex[3] = Vertex(PointPixelToVulkan({this->_pos[0], this->_pos[1] + this->_size[1]}, screenSize),
									  {0.0f, 1.0f}, this->_colorMod, E_ImageType::Color);
		}
	}

	void Image::Draw()
	{
		auto cmdBuffer =
			Rendering::CommandsPool::GetInstance().GetBuffer(Rendering::SyncObjects::GetInstance().GetCurrentFrame());
		VkDeviceSize offsets[] = {0};

		vkCmdPushConstants(cmdBuffer,
						   Front::Rendering::Pipelines::PipelinesManager::GetInstance()
							   .operator[]<Front::Rendering::Pipelines::StaticGUIPipeline>("StaticGUI")
							   ->GetLayout(),
						   VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(this->_uvMappingData), &this->_uvMappingData);
		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &this->B_Vertices->GetBuffer(0), offsets);
		vkCmdBindIndexBuffer(cmdBuffer, this->B_Indices->GetBuffer(0), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);
	}

	void Image::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		const Vector2 screenSize(Rendering::SwapChain::GetInstance().GetExtent().width,
								 Rendering::SwapChain::GetInstance().GetExtent().height);
		// this->ResetVertex();
		this->_vertex[0].position = PointPixelToVulkan(this->_pos, screenSize);
		this->_vertex[1].position = PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1]}, screenSize);
		this->_vertex[2].position =
			PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1] + this->_size[1]}, screenSize);
		this->_vertex[3].position = PointPixelToVulkan({this->_pos[0], this->_pos[1] + this->_size[1]}, screenSize);
		this->B_Vertices->Update(&this->_vertex, 4 * sizeof(Vertex));
	}

	void Image::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		this->ResetVertex();
		this->B_Vertices->Update(&this->_vertex, 4 * sizeof(Vertex));
	}

	void Image::SetColor(const Color &newColor)
	{
		if (this->B_Vertices == VK_NULL_HANDLE || this->_vertex[0].texColor == newColor)
			return;
		for (auto &vert : this->_vertex)
			vert.texColor = newColor;
		this->B_Vertices->Update(&this->_vertex, 4 * sizeof(Vertex));
	}

	void Image::SetTexture(const std::string &newAtlas, const std::string &newKey)
	{
		if (newAtlas == this->_atlas && newKey == this->_atlasKey)
			return;
		this->_atlas = newAtlas;
		this->_atlasKey = newKey;

		auto atlas = Game::GameManager::GetInstance().GetTexturesManager()[_atlas];
		auto uvData = atlas->GetTextureInfo(this->_atlasKey);

		this->_uvMappingData.uvMin[0] = uvData.uOffset;
		this->_uvMappingData.uvMin[1] = uvData.vOffset;
		this->_uvMappingData.uvMax[0] = uvData.uOffset + uvData.uSize;
		this->_uvMappingData.uvMax[1] = uvData.vOffset + uvData.vSize;
		this->_uvMappingData.atlasSize = atlas->_mainAtlas->GetSize();

		this->B_Vertices->Update(&this->_vertex, 4 * sizeof(Vertex));
	}
} // namespace Vox::Front::Interfaces::Elements