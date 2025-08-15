#ifndef __ACONTAINER_HPP__
#define __ACONTAINER_HPP__

#include "AElement.hpp"
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

namespace Vox::Front::Interfaces::Elements::Bases
{
    struct ContainerElement
    {
        std::string key;
        std::unique_ptr<AElement> elem;
        size_t renderIndex;

        ContainerElement() = delete;

        // Constructeur prenant un unique_ptr
        ContainerElement(std::string key_, std::unique_ptr<AElement> elem_, size_t renderIndex_ = 0)
            : key(std::move(key_)), elem(std::move(elem_)), renderIndex(renderIndex_) {}

        // Plus besoin de destructeur : unique_ptr gère tout
        ~ContainerElement() = default;

        // Interdiction de copie pour éviter double gestion mémoire
        ContainerElement(const ContainerElement&) = delete;
        ContainerElement& operator=(const ContainerElement&) = delete;

        // Autorisation du move
        ContainerElement(ContainerElement&&) noexcept = default;
        ContainerElement& operator=(ContainerElement&&) noexcept = default;
    };

    class AContainer : public AElement
    {
    public:
        AContainer(Vector2 pos = {0, 0}, Vector2 size = {100, 50})
            : AElement(pos, size) {}

        virtual ~AContainer() = default;

        inline void AddElement(const std::string &key, std::unique_ptr<AElement> elem, size_t renderIndex = 0)
        {
            if (this->FindElement(key) != this->_content.end())
                throw std::runtime_error("Trying to add an already existing element.");

            _content.emplace_back(key, std::move(elem), renderIndex);

            if (_content.size() > 1 && _content.back().renderIndex < _content[_content.size() - 2].renderIndex)
            {
                std::sort(_content.begin(), _content.end(),
                          [](ContainerElement &elem1, ContainerElement &elem2)
                          { return elem1.renderIndex < elem2.renderIndex; });
            }
        }

        inline bool RemoveElement(const std::string &key)
        {
            auto newEnd = std::remove_if(_content.begin(), _content.end(),
                                         [key](const ContainerElement &elem) { return elem.key == key; });
            bool found = newEnd != _content.end();
            _content.erase(newEnd, _content.end());
            return found;
        }

        template <typename T>
        T *GetElement(const std::string &key)
        {
            return dynamic_cast<T *>(this->GetContainerElement(key).elem.get());
        }

        inline ContainerElement &GetContainerElement(const std::string &key)
        {
            auto it = this->FindElement(key);
            if (it == this->_content.end())
                throw std::runtime_error("Element \"" + key + "\" not found.");
            return *it;
        }

        inline void Draw()
        {
            for (ContainerElement &cElem : this->_content)
                if (cElem.elem != nullptr)
                    cElem.elem->Draw();
        }

        virtual void SetPos(const Vector2 newPos) = 0;
        virtual void SetSize(const Vector2 newSize) = 0;

    protected:
        std::vector<ContainerElement> _content;

        inline std::vector<ContainerElement>::iterator FindElement(const std::string &key)
        {
            return std::find_if(this->_content.begin(), this->_content.end(),
                                [key](ContainerElement &elem) { return elem.key == key; });
        }
    };
} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __ACONTAINER_HPP__
