#include "primitive.hpp"

namespace x::render::drawable
{
    void Primitive::SetVertices(const void* data, const size_t size, const size_t stride)
    {
        auto hr = S_OK;

        m_stride = stride;
        m_size = size;
        m_vertexCount = size / stride;

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = static_cast<UINT>(m_size);
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = data;

        hr = m_device->CreateBuffer(&bufferDesc, &initData, m_vertexBuffer.GetAddressOf()) HTHROW("Failed to create vertex buffer");
    }
}
