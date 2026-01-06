#ifndef __TEXT_HPP__
#define __TEXT_HPP__

#include "./Bases/AElement.hpp"

#include "MathGraphicalLib/Vectors/Vector4.hpp"

using E_ImageType = Vox::Front::Rendering::Utils::Vertex::E_ImageType;

namespace Vox::Front::Interfaces::Elements
{
	class Text : public Bases::AElement
	{

		public:
			struct Vox_Text_Constructor
			{
					Vector2Float pos;
					Vector2Float size;
					Color color;
					std::string content;
					float scale;
					float letterSpace;

					Vox_Text_Constructor()
						: pos({0, 0}), size({100, 50}), color({
															1.0,
															1.0,
															1.0,
															1.0,
														}),
						  content(""), scale(0.5), letterSpace(0) {};
			};

			Text() = delete;
			Text(const Vox_Text_Constructor &st);
			Text(Vector2Float pos = {0, 0}, Vector2Float size = {100, 50}, Color color = {1.0f, 1.0f, 1.0f, 1.0f},
				 std::string content = "", float scale = 0.5);
			~Text();

			void ResetVertex() override;
			void Draw() override;
			void SetPos(const Vector2Float newPos) override;
			void SetSize(const Vector2Float newSize) override;

			void SetColor(const Color &newColor);

			void SetContent(const std::string &newContent);

			static Vector2Float GetTextSize(const std::string &content, const float &scale, const float letterSpace = 0.0f);
			Vector2Float GetTextSize();
			const std::string &GetContent() {return this->_textContent;};

		private:
			size_t _indexCount = 0;
			std::vector<Vertex> _vertex;
			dbuffer *B_Vertex = nullptr;
			dbuffer *B_Index = nullptr;

			Color _textColor;
			std::string _textContent;
			float _scale;
			float _letterSpacing;

			void CleanBuffer(size_t mode);
			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __TEXT_HPP__