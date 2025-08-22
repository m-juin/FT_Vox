#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include "Bases/AElement.hpp"

#include "Front/Rendering/Utils/Vertex/StaticGUIVertex.hpp"

#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"

using vert = Vox::Front::Rendering::Utils::Vertex::StaticGUIVertex;
using E_ImageType = Vox::Front::Rendering::Utils::Vertex::E_ImageType;
using buffer = Vox::Front::Rendering::Utils::Buffers::StaticBuffer;

namespace Vox::Front::Interfaces::Elements
{
	class Image : public Bases::AElement
	{
		private:
			vert vertex[4];
			buffer *B_Vertices = nullptr;
			buffer *B_Indices = nullptr;
			/* data */

            void CleanBuffers(size_t mode);
            void ResetVertex();

			std::string _atlas;
			std::string _atlasKey;

		public:
			// Image(Vector2 pos = {0, 0}, Vector2 size = {100, 50});
			Image(std::string atlas = "", std::string key = "", Bases::Vector2 pos = {0, 0}, Bases::Vector2 size = {100, 50});
			~Image();

			void Draw();
			void SetPos(const Bases::Vector2 newPos);
			void SetSize(const Bases::Vector2 newSize);
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __IMAGE_HPP__