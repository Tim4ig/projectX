#pragma once

#include "node.hpp"
#include "resources/constant.hpp"
#include "resources/drawable/mesh.hpp"

namespace x::render::drawable
{
    class Drawable
    {
    public:
        explicit Drawable(const ComPtr<ID3D11Device>& device) : m_device(device), m_root(device)
        {
        }

        ComPtr<ID3D11Device> m_device;

        Node m_root;
        std::vector<Mesh> m_meshes;

    private:
        friend class Renderer;
    };
}
