#include "Game/Scenes/World/Skybox/SkyBox.hpp"

#include "Front/Rendering/CommandsPool.hpp"
#include "Front/Rendering/Pipelines/PipelinesManager.hpp"
#include "Front/Rendering/Pipelines/SkyBoxPipeline.hpp"
#include "Front/Rendering/SyncObjects.hpp"

namespace Vox::Game::World::Skybox
{
	SkyBox::SkyBox()
	{
		this->_vertex = {Vector3(-1.0f, 1.0f, -1.0f), Vector3(-1.0f, -1.0f, -1.0f), Vector3(1.0f, -1.0f, -1.0f),
						 Vector3(1.0f, 1.0f, -1.0f),  Vector3(-1.0f, 1.0f, 1.0f),	Vector3(-1.0f, -1.0f, 1.0f),
						 Vector3(1.0f, -1.0f, 1.0f),  Vector3(1.0f, 1.0f, 1.0f)};
		this->_index = {// face arrière
						0, 1, 2, 2, 3, 0,
						// face avant
						4, 5, 6, 6, 7, 4,
						// gauche
						4, 5, 1, 1, 0, 4,
						// droite
						3, 2, 6, 6, 7, 3,
						// haut
						4, 0, 3, 3, 7, 4,
						// bas
						1, 5, 6, 6, 2, 1};

		this->B_Index = new sbuffer(1, _index.size() * sizeof(uint16_t), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		this->B_Index->Create(_index.data());
		this->B_Vertex = new sbuffer(1, _vertex.size() * sizeof(Vertex), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		this->B_Vertex->Create(this->_vertex.data());
	}

	void SkyBox::Render()
	{
		using namespace Front::Rendering;
		auto frame = SyncObjects::GetInstance().GetCurrentFrame();
		auto buffer = CommandsPool::GetInstance().GetBuffer(frame);

		auto pipeline = Pipelines::PipelinesManager::GetInstance().operator[]<Pipelines::SkyBoxPipeline>("Skybox");
		if (pipeline == nullptr)
			return;
		VkDeviceSize offset = {0};
		vkCmdBindDescriptorSets(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1,
								&pipeline->GetSet(0), 0, 0);
		vkCmdBindVertexBuffers(buffer, 0, 1, &this->B_Vertex->GetBuffer(0), &offset);
		vkCmdBindIndexBuffer(buffer, this->B_Index->GetBuffer(0), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(buffer, this->_index.size(), 1, 0, 0, 0);
	} // namespace Vox::Game::World::Skybox

	SkyBox::~SkyBox()
	{

	}

} // namespace Vox::Game::World::Skybox