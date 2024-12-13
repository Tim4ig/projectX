#pragma once

#include <dxgi1_2.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "pipelines/pipeline_debug.hpp"
#include "resources/camera.hpp"
#include "resources/drawable/drawable.hpp"

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class Renderer
    {
    public:
        Renderer(const ComPtr<ID3D11Device>& device, HWND window);
        ~Renderer();

        void SetResolution(POINT size);

        void BeginFrame();
        void EndFrame();

        void Draw(const Drawable* drawable);

        void Bind(Camera& camera) const;

    private:
        HWND m_window;

        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

        ComPtr<IDXGIFactory2> m_dxgiFactory;
        ComPtr<IDXGISwapChain1> m_swapChain;

        D3D11_VIEWPORT m_viewport;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;

        std::vector<const Drawable*> m_renderQueue;

        bool m_framestate = false;

        std::unique_ptr<pipeline::PipelineDebug> m_pipelineDebug;

        struct
        {
            bool vsync = false;
            float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        } m_settings;

        void m_Init();
        void m_InitSwapChain();
        void m_InitWindowStyles() const;
        void m_InitBuffers();
        void m_InitPipelines();
    };
}
