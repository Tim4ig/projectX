#include "d3d11mesh.hpp"

#include "pch.h"

namespace x::render
{
    void Mesh::m_SetVertices(const void* vertices, const UINT vertexCount, const UINT vertexSize)
    {
        auto hr = S_OK;

        m_vertexCount = vertexCount;
        m_vertexSize = vertexSize;
        m_stride = vertexSize / vertexCount;

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = vertexCount * vertexSize;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem = vertices;

        hr = m_device->CreateBuffer(&bufferDesc, &subresourceData, &m_vertexBuffer) HTHROW("Failed to create vertex buffer");
    }
}
