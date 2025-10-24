#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include <vector>
#include <cstdint>

#include "Front/Rendering/Utils/Vertex/SkyBoxVertex.hpp"
#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"

namespace Vox::Game::World::Skybox
{
    using Vertex = Front::Rendering::Utils::Vertex::SkyBoxVertex;
    using Vector3 = Front::Rendering::Utils::Vertex::Vector3Float;
    using sbuffer = Vox::Front::Rendering::Utils::Buffers::StaticBuffer;
	class SkyBox
	{
		public:
			SkyBox();
			~SkyBox();
            void Render();

		private:
			std::vector<uint16_t> _index;
			std::vector<Vertex> _vertex;

			sbuffer *B_Vertex;
			sbuffer *B_Index;
			/* private */
	};
} // namespace Vox::Game::World::Skybox

#endif // __SKYBOX_HPP__