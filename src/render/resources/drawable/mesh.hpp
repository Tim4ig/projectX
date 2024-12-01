#pragma once

#include "material.hpp"
#include "primitive.hpp"
#include "pch.h"

namespace x::render::drawable
{
    class Mesh
    {
    public:
        explicit Mesh(const ComPtr<ID3D11Device>& device) : m_device(device)
        {
        }

        std::vector<Primitive> m_primitives;

    private:
        ComPtr<ID3D11Device> m_device;


        friend class Renderer;
    };
}
