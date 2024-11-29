#include "d3d11constant.hpp"

#include "pch.h"

namespace x::render
{
    void ConstantBuffer::SetConstantData(const void* data, const UINT dataSize)
    {
        auto hr = S_OK;

        if (!m_buffer || m_dataSize != dataSize)
            m_CreateBuffer(dataSize);

        ComPtr<ID3D11DeviceContext> context;
        m_device->GetImmediateContext(&context) HTHROW("Failed to get immediate context");

        D3D11_MAPPED_SUBRESOURCE mappedResource = {};
        hr = context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) HTHROW("Failed to map constant buffer");
        memcpy(mappedResource.pData, data, dataSize);
        context->Unmap(m_buffer.Get(), 0);
    }

    void ConstantBuffer::m_CreateBuffer(const UINT dataSize)
    {
        auto hr = S_OK;

        m_dataSize = dataSize;

        D3D11_BUFFER_DESC desc = {};
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = dataSize + 0xF & ~0xF;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        hr = m_device->CreateBuffer(&desc, nullptr, &m_buffer) HTHROW("Failed to create constant buffer");
    }
}
