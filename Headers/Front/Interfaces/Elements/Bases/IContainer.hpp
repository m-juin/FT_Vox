#ifndef __INDEXCONTAINER_HPP__
#define __INDEXCONTAINER_HPP__

#include "AClickable.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

namespace Vox::Front::Interfaces::Elements::Bases
{
    struct IndexedContainerElement
    {
        std::unique_ptr<AElement> elem;
        size_t renderIndex;
        
        IndexedContainerElement() = delete;
        IndexedContainerElement(std::unique_ptr<AElement> elem_, size_t renderIndex_ = 0)
            : elem(std::move(elem_)), renderIndex(renderIndex_)
        {
        }
        
        ~IndexedContainerElement() = default;
        IndexedContainerElement(const IndexedContainerElement&) = delete;
        IndexedContainerElement& operator=(const IndexedContainerElement&) = delete;
        IndexedContainerElement(IndexedContainerElement&&) noexcept = default;
        IndexedContainerElement& operator=(IndexedContainerElement&&) noexcept = default;
    };

    class IContainer : public virtual AClickable
    {
    public:
        IContainer() {}
            
        virtual ~IContainer() {}
        void ResetVertex() override {}
        
        void AddElement(std::unique_ptr<AElement> elem, size_t renderIndex = 0);
        bool RemoveElement(size_t index);
        
        template<typename T>
        T* GetElement(size_t index)
        {
            if (index >= _content.size())
                return nullptr;
            return dynamic_cast<T*>(_content[index].elem.get());
        }
        
        IndexedContainerElement& GetContainerElement(size_t index)
        {
            if (index >= _content.size())
                throw std::out_of_range("Index hors limites");
            return _content[index];
        }
        
        virtual void Draw() override;
        virtual void OnHover() override;
        virtual void OnClick(const int& button, const int& action) override;
        virtual void OnHoverLeave() override;
        virtual bool IsHover(const Vector2& mousePos, bool override = false) override;
        
    protected:
        Vox::Utils::CallBacksManager<AElement&> onElementAdd;
        Vox::Utils::CallBacksManager<> onElementRemove;
        std::vector<IndexedContainerElement> _content;
    };
} // namespace Vox::Front::Interfaces::Elements::Bases

#endif // __INDEXCONTAINER_HPP__