#pragma once

#include "resources/camera.hpp"
#include "resources/constant.hpp"
#include "resources/shader.hpp"
#include "resources/texture.hpp"
#include "resources/drawable/drawable.hpp"

namespace x::render::pipeline
{
    class MainPipeline
    {
    public:
        explicit MainPipeline(const ComPtr<ID3D11Device>& device, const ComPtr<IDXGISwapChain>& swapChain) : m_device(device), m_swapChain(swapChain), m_constantBuffer(device)
        {
            m_device->GetImmediateContext(&m_context);
            m_Init();
        }

        void Resize(POINT size);
        void SetClearColor(unsigned int rgba);

        void Clear() const;
        void BeginFrame();
        void EndFrame();

        void Draw(const std::vector<const Drawable*>& queue) const;

        void Bind(const Shader& shader) const;
        void Bind(const ConstantBuffer& constantBuffer, int slot) const;
        void Bind(const Texture& texture, int slot) const;
        void Bind(Camera& camera);

    private:
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;
        ComPtr<IDXGISwapChain> m_swapChain;

        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;

        ComPtr<ID3D11SamplerState> m_samplerState;

        ConstantBuffer m_constantBuffer;
        D3D11_VIEWPORT m_viewport;

        float m_clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

        void m_Init();
        void m_InitBuffers();
    };
}
