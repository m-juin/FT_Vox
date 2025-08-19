#ifndef __IMAGE_HPP__
#define __IMAGE_HPP__

#include "Bases/AElement.hpp"

#include "Front/Rendering/Utils/Vertex/StaticGUIVertex.hpp"

#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"

using vert = Vox::Front::Rendering::Utils::Vertex::StaticGUIVertex;
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

		public:
			Image(Vector2 pos = {0, 0}, Vector2 size = {100, 50});
			~Image();

			void Draw();
			void SetPos(const Vector2 newPos);
			void SetSize(const Vector2 newSize);
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __IMAGE_HPP__