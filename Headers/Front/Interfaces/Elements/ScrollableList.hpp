#ifndef __SCROLLABLELIST_HPP__
#define __SCROLLABLELIST_HPP__

#include "Front/Interfaces/Elements/Bases/IContainer.hpp"
#include "Front/Interfaces/Elements/Bases/AScrollable.hpp"

namespace Vox::Front::Interfaces::Elements
{
	class ScrollableList : public Bases::IContainer, Bases::AScrollable
	{
		public:
			struct Vox_ScrollableList_Constructor
			{
				Vector2 pos;
				Vector2 size;
                
                // bool enableDeletion;
                std::vector<std::unique_ptr<AElement>> content;
			};

			ScrollableList() = delete;
			ScrollableList(Vox_ScrollableList_Constructor &st) ;

            void OnEnable() override {};
			void OnDisable() override {};
			void OnHover() override {};
			void OnClick(const int &button, const int &action) override {(void)button; (void)action;};
			void OnHoverLeave() override {};

			using AClickable::IsHover;
			

			void SetPos(const Vector2 newPos) override;
			void SetSize(const Vector2 newSize) override;

            void Draw() override;
			~ScrollableList() {};
		private:

            float _scrollOffset = 0;
			int _fullSize = 0;
            void RebuildList();
            // bool _enableDeletion;
			/* private */
	};
} // namespace Vox::Front::Interfaces::Elements

#endif // __SCROLLABLELIST_HPP__