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
        explicit IPipeline(const ComPtr<ID3D11Device>& device);

        virtual ~IPipeline() = default;

        virtual void Resize(POINT size);

        virtual void Clear() const;

        virtual void BeginFrame(const ComPtr<ID3D11RenderTargetView>& rtv, const ComPtr<ID3D11DepthStencilView>& dsv);
        virtual ComPtr<ID3D11CommandList> EndFrame();

        virtual void Draw(const std::vector<const Drawable*>& queue) const;

        virtual void Bind(const drawable::Material& material) const;
        virtual void Bind(const Shader& shader) const;
        virtual void Bind(const ConstantBuffer& constantBuffer, int slot) const;
        virtual void Bind(const Texture& texture, int slot) const;
        virtual void Bind(Camera& camera);

    protected:
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

        D3D11_VIEWPORT m_viewport = {};
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;

        ConstantBuffer m_constantBuffer;
        std::unique_ptr<Shader> m_shader;

        float m_clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

        virtual void m_Init() = 0;
        virtual void m_InitShader() = 0;
    };
}
