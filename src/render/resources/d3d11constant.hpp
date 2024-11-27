#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class ConstantBuffer
    {
    public:
        explicit ConstantBuffer(const ComPtr<ID3D11Device>& device) : m_device(device)
        {
        }

        virtual ~ConstantBuffer() = default;

    private:
        ComPtr<ID3D11Device> m_device;

        UINT m_dataSize = 0;
        ComPtr<ID3D11Buffer> m_buffer;

        void m_CreateBuffer(UINT dataSize);

        friend class Renderer;

    protected:
        void m_SetData(const void* data, UINT dataSize);
    };
}
