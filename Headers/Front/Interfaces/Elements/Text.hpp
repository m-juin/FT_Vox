#ifndef __TEXT_HPP__
#define __TEXT_HPP__

#include "./Bases/AElement.hpp"

#include "MathGraphicalLib/Vectors/Vector4.hpp"

#include "Front/Rendering/Utils/Buffers/StaticBuffer.hpp"

using E_ImageType = Vox::Front::Rendering::Utils::Vertex::E_ImageType;
using buffer = Vox::Front::Rendering::Utils::Buffers::StaticBuffer;

namespace Vox::Front::Interfaces::Elements
{
	class Text : public Bases::AElement
	{

		public:
			struct Vox_Text_Constructor
			{
					Vector2 pos;
					Vector2 size;
					Color color;
					std::string content;
					float scale;

					Vox_Text_Constructor()
						: pos({0, 0}), size({100, 50}), color({
															1.0,
															1.0,
															1.0,
															1.0,
														}),
						  content(""), scale(0.5) {};
			};

			Text() = delete;
			Text(const Vox_Text_Constructor &st);
			Text(Vector2 pos = {0, 0}, Vector2 size = {100, 50}, Color color = {1.0f, 1.0f, 1.0f, 1.0f},
				 std::string content = "", float scale = 0.5);
			~Text();

			void ResetVertex() override;
			void Draw() override;
			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;

			static Vector2 GetTextSize(const std::string &content, const float &scale);

		private:
			std::vector<Vertex> _vertex;
			buffer *B_Vertex = nullptr;
			buffer *B_Index = nullptr;

			Color _textColor;
			std::string _textContent;
			float _scale;

			void CleanBuffer(size_t mode);
			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __TEXT_HPP__