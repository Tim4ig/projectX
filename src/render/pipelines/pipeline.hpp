#pragma once

#include "resources/camera.hpp"
#include "resources/constant.hpp"
#include "resources/shader.hpp"
#include "resources/drawable/drawable.hpp"
#include "resources/drawable/material.hpp"
#include "resources/drawable/texture.hpp"

namespace x::render::pipeline
{
    class IPipeline
    {
    public:
        virtual ~IPipeline() = default;

        virtual void Resize(POINT size) = 0;

        virtual void Clear() const = 0;

        virtual void BeginFrame(const ComPtr<ID3D11RenderTargetView>& rtv, const ComPtr<ID3D11DepthStencilView>& dsv) = 0;
        virtual ComPtr<ID3D11CommandList> EndFrame() = 0;

        virtual void Draw(const std::vector<const Drawable*>& queue) const = 0;

        virtual void Bind(const drawable::Material& material) const = 0;
        virtual void Bind(const Shader& shader) const = 0;
        virtual void Bind(const ConstantBuffer& constantBuffer, int slot) const = 0;
        virtual void Bind(const Texture& texture, int slot) const = 0;
        virtual void Bind(Camera& camera) = 0;
    };
}
