#include "mesh.hpp"

#include "pch.h"

namespace x::render
{
    void Mesh::SetVertices(const void* vertices, const UINT vertexCount, const UINT size)
    {
        auto hr = S_OK;

        m_vertexCount = vertexCount;
        m_size = size;
        m_stride = size / vertexCount;

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = size;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem = vertices;

        hr = m_device->CreateBuffer(&bufferDesc, &subresourceData, &m_vertexBuffer) HTHROW("Failed to create vertex buffer");
    }
}
