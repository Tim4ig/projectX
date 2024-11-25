
#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include "d3d11mesh.hpp"

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class Drawable : public Mesh
    {
    public:
        Drawable(const ComPtr<ID3D11Device>& device) : Mesh(device), m_device(device) {}
        virtual ~Drawable() = default;
    private:
        ComPtr<ID3D11Device> m_device;
        friend class Renderer;
    };
}
