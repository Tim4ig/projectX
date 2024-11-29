#pragma once

#include "resources/drawable.hpp"

namespace x::world
{
    class DrawableWrapper
    {
    public:
        explicit DrawableWrapper(std::unique_ptr<render::Drawable>& drawable) : m_drawable(std::move(drawable))
        {
        }

        virtual ~DrawableWrapper() = default;

        void m_SetVertecis(const void* rawMem, const UINT64 count, const UINT64 size) const
        {
            m_drawable->SetVertices(rawMem, count, size);
        }

        void m_SetTexture(const fs::Texture& texture) const
        {
            m_drawable->SetTexture(texture);
        }

        operator render::Drawable& () const
        {
            return *m_drawable;
        }

    private:
        std::unique_ptr<render::Drawable> m_drawable;

    protected:

        void m_SetConstantBuffer(const void* rawMem, const UINT64 size) const
        {
            m_drawable->SetConstantData(rawMem, size);
        }

    };
}
