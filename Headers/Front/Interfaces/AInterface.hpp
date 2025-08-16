#ifndef __AINTERFACE_HPP__
#define __AINTERFACE_HPP__

#include "Front/Interfaces/Elements/Bases/AContainer.hpp"

namespace Vox::Front::Interfaces
{
	class AInterface : public Elements::Bases::AContainer
	{
		public:
			AInterface(Vector2 pos = {0, 0}, Vector2 size = {0, 0}) : AContainer(pos, size) {};
			~AInterface() {};

			virtual void Render() = 0;
			virtual void SetPos(const Vector2 newPos) = 0;
			virtual void SetSize(const Vector2 newSize) = 0;

		private:
			/* private */
	};
} // namespace Vox::Front::Interfaces

#endif // __AINTERFACE_HPP__