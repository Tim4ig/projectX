#pragma once

#include "pch.h"

#include "filesystem/textures/textureT.hpp"

namespace x::render
{
    class Texture
    {
    public:
        Texture(const ComPtr<ID3D11Device>& device) : m_device(device)
        {
        }

        ~Texture() = default;

        void SetTexture(const fs::Texture& texture);

    private:
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11Texture2D> m_texture;
        ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

        friend class Renderer;
    };
}
