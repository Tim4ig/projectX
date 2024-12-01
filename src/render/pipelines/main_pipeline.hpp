#pragma once

#include "pipeline.hpp"
#include "resources/camera.hpp"
#include "resources/constant.hpp"
#include "resources/shader.hpp"
#include "resources/drawable/texture.hpp"
#include "resources/drawable/drawable.hpp"

namespace x::render::pipeline
{
    class MainPipeline : public IPipeline
    {
    public:
        explicit MainPipeline(const ComPtr<ID3D11Device>& device) : m_device(device), m_constantBuffer(device)
        {
            m_device->GetImmediateContext(&m_context);
            m_Init();
        }

        void BeginFrame(const ComPtr<ID3D11RenderTargetView>& rtv, const ComPtr<ID3D11DepthStencilView>& dsv) override;
        void EndFrame() override;

        void Draw(const std::vector<const Drawable*>& queue) const override;

        void Bind(const drawable::Material& material) const override;
        void Bind(const Shader& shader) const override;
        void Bind(const ConstantBuffer& constantBuffer, int slot) const override;
        void Bind(const Texture& texture, int slot) const override;
        void Bind(Camera& camera) override;

    private:
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;

        ComPtr<ID3D11SamplerState> m_samplerState;

        ConstantBuffer m_constantBuffer;
        D3D11_VIEWPORT m_viewport;

        std::unique_ptr<Shader> m_shader;

        float m_clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};

        void m_Init();
        void m_InitShader();
    };
}
