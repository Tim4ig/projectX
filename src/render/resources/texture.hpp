#pragma once

#include "filesystem/textures/texture.hpp"

namespace x::render
{
    class Texture
    {
    public:
        explicit Texture(const ComPtr<ID3D11Device>& device) : m_device(device)
        {
        }

        void SetTexture(const fs::Texture& texture);

        virtual ~Texture() = default;

    private:
        ComPtr<ID3D11Device> m_device;

        ComPtr<ID3D11Texture2D> m_texture;
        ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

        friend class Renderer;
    };
}
