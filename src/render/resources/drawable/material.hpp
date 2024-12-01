#pragma once

namespace x::render::drawable
{
    class Material
    {
    public:
        explicit Material(const ComPtr<ID3D11Device>& device) : m_device(device)
        {
        }

    private:
        ComPtr<ID3D11Device> m_device;

        friend class Renderer;
    };
}
