#ifndef __INTERFACESMANAGER_HPP__
#define __INTERFACESMANAGER_HPP__

#include "Utils/Singleton.hpp"

#include <unordered_map>

#include "AInterface.hpp"

namespace Vox::Front::Interfaces
{
	class InterfacesManager : public Vox::Utils::Singleton<InterfacesManager>
	{
			friend class Vox::Utils::Singleton<InterfacesManager>;

		public:
			void ResetInterfacesList();
			void RegisterInterface(const std::string &key, AInterface *inte);
			void Render();

			void HandleMouseMove(const float &xPos, const float &yPos) const;
			void HandleMouseClick(const int &button, const int &action) const;
			void HandleMouseScroll(const double &xOff, const double &yOff) const;
			void HandleCharInput(const unsigned int &codePoint) const;
			void HandleKeyInput(const unsigned int &codePoint, const int &action) const;
			InterfacesManager();
			~InterfacesManager();

			// Version non-constante pour les modifications
			AInterface *&operator[](const std::string &key)
			{
				return _content[key];
			}
			// Ajoutez ces fonctions pour une meilleure gestion des états
			void EnableInterface(const std::string &key)
			{
				auto &interface = _content[key];
				interface->ChangeEnableState(true);
			}

			void DisableInterface(const std::string &key)
			{
				auto &interface = _content[key];
				interface->ChangeEnableState(false);
			}

			bool IsInterfaceEnabled(const std::string &key)
			{
				return _content[key]->IsEnabled();
			}

		private:
			;
			std::unordered_map<std::string, AInterface *> _content;
			/* private */
	};
} // namespace Vox::Front::Interfaces

#endif // __INTERFACESMANAGER_HPP__