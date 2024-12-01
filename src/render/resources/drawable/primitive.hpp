#pragma once

#include "material.hpp"

namespace x::render::drawable
{
    class Primitive
    {
    public:
        explicit Primitive(const ComPtr<ID3D11Device>& device) : m_material(device), m_device(device)
        {
        }

        void SetVertices(const void* data, size_t size, size_t stride);
        Material m_material;
        ComPtr<ID3D11Buffer> m_vertexBuffer;
        size_t m_stride = 0;
        size_t m_size = 0;
        size_t m_vertexCount = 0;

    private:
        ComPtr<ID3D11Device> m_device;
    };
}
