#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class Mesh
    {
    public:
        explicit Mesh(const ComPtr<ID3D11Device>& device) : m_device(device)
        {
        }

        virtual ~Mesh() = default;

        void SetVertices(const void* vertices, UINT vertexCount, UINT size);

    private:
        ComPtr<ID3D11Device> m_device;

        UINT m_stride = 0;
        UINT m_size = 0;
        UINT m_vertexCount = 0;
        ComPtr<ID3D11Buffer> m_vertexBuffer;

        friend class Renderer;
    };
}
