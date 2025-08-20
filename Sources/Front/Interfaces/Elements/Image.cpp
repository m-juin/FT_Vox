#include "Front/Interfaces/Elements/Image.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/SwapChain.hpp"
#include "Front/Rendering/SyncObjects.hpp"

#include "Front/Interfaces/Utils/Maths.hpp"

namespace Vox::Front::Interfaces::Elements
{
	Image::Image(Vector2 pos, Vector2 size) : Bases::AElement(pos, size)
	{
		this->ResetVertex();

		B_Vertices = new buffer(1, 4 * sizeof(vert), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
		B_Indices = new buffer(1, 6 * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);

		B_Vertices->Create(&this->vertex);

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
		if ((mode == 0 || mode == 2) && this->B_Vertices)
			delete this->B_Vertices;
		if (mode >= 1 && this->B_Indices)
			delete this->B_Indices;
	}

	void Image::ResetVertex()
	{
		this->CleanBuffers(0);

		std::cout << this->_pos << std::endl;
		std::cout << this->_size << std::endl;

		const Vector2 screenSize(Rendering::SwapChain::GetInstance().GetExtent().width,
								 Rendering::SwapChain::GetInstance().GetExtent().height);

		this->vertex[0] =
			vert(Utils::Maths::PointPixelToVulkan(this->_pos, screenSize), {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
		this->vertex[1] =
			vert(Utils::Maths::PointPixelToVulkan({this->_pos[0] + this->_size[0], this->_pos[1]}, screenSize),
				 {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
		this->vertex[2] = vert(Utils::Maths::PointPixelToVulkan(
								   {this->_pos[0] + this->_size[0], this->_pos[1] + this->_size[1]}, screenSize),
							   {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
		this->vertex[3] =
			vert(Utils::Maths::PointPixelToVulkan({this->_pos[0], this->_pos[1] + this->_size[1]}, screenSize),
				 {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f});

	}

	void Image::Draw()
	{
		auto cmdBuffer =
			Rendering::CommandsPool::GetInstance().GetBuffer(Rendering::SyncObjects::GetInstance().GetCurrentFrame());
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &this->B_Vertices->GetBuffer(0), offsets);
		vkCmdBindIndexBuffer(cmdBuffer, this->B_Indices->GetBuffer(0), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(cmdBuffer, 6, 1, 0, 0, 0);
	}

	void Image::SetPos(const Vector2 newPos)
	{
		if (newPos == this->_pos)
			return;
		this->_pos = newPos;
		this->ResetVertex();
	}

	void Image::SetSize(const Vector2 newSize)
	{
		if (newSize == this->_size)
			return;
		this->_size = newSize;
		CleanBuffers(0);
		this->ResetVertex();
	}
} // namespace Vox::Front::Interfaces::Elements