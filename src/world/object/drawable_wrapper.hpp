#pragma once

#include "resources/d3d11drawable.hpp"

namespace x::world
{
    class DrawableWrapper : public render::Drawable
    {
    public:
        explicit DrawableWrapper(const ComPtr<ID3D11Device>& device) : Drawable(device)
        {
        }

        void SetVertecis(const void* rawMem, const UINT64 count, const UINT64 stride)
        {
            m_SetVertices(rawMem, count, stride);
        }

        void SetConstantBuffer(const void* rawMem, const UINT64 size)
        {
            m_SetData(rawMem, size);
        }

        virtual ~DrawableWrapper() override = default;
    };
}
