#pragma once

#include "mesh.hpp"
#include "constant.hpp"

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class Drawable : public Mesh, public ConstantBuffer
    {
    public:
        explicit Drawable(const ComPtr<ID3D11Device>& device) : Mesh(device), ConstantBuffer(device), m_device(device)
        {
        }

        ~Drawable() override = default;

    private:
        ComPtr<ID3D11Device> m_device;
        friend class Renderer;
    };
}
