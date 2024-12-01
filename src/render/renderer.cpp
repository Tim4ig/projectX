#include "renderer.hpp"

#include "pch.h"

namespace x::render
{
    Renderer::Renderer(const ComPtr<ID3D11Device>& device, const HWND window) : m_window(window), m_device(device)
    {
        m_InitWindowStyles();
        m_Init();
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::SetClearColor(const unsigned int rgba)
    {
        m_settings.clearColor[0] = static_cast<float>(rgba >> 24 & 0xFF) / 255.0f;
        m_settings.clearColor[1] = static_cast<float>(rgba >> 16 & 0xFF) / 255.0f;
        m_settings.clearColor[2] = static_cast<float>(rgba >> 8 & 0xFF) / 255.0f;
        m_settings.clearColor[3] = static_cast<float>(rgba & 0xFF) / 255.0f;
    }

    void Renderer::Clear() const
    {
        if (m_framestate == false)
            XTHROW("frame not started");

        m_context->ClearRenderTargetView(m_renderTargetView.Get(), m_settings.clearColor);
        m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void Renderer::SetResolution(const POINT size)
    {
        if (m_framestate == true)
            XTHROW("frame already started");

        m_viewport.Width = static_cast<FLOAT>(size.x);
        m_viewport.Height = static_cast<FLOAT>(size.y);

        m_context->OMSetRenderTargets(0, nullptr, nullptr);
        m_renderTargetView.Reset();
        m_depthStencilView.Reset();

        auto hr = S_OK;
        hr = m_swapChain->ResizeBuffers(0, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height), DXGI_FORMAT_UNKNOWN, 0) HTHROW("failed to resize buffers");

        SetWindowPos(m_window, nullptr, 0, 0, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height), SWP_NOMOVE | SWP_NOZORDER);

        m_InitBuffers();
    }

    void Renderer::BeginFrame()
    {
        if (m_framestate == true)
            XTHROW("frame already started");
        m_framestate = true;
    }

    void Renderer::EndFrame()
    {
        if (m_framestate == false)
            XTHROW("frame already ended");

        auto hr = S_OK;

        m_context->RSSetViewports(1, &m_viewport);

        {
            m_mainPipeline->BeginFrame(m_renderTargetView, m_depthStencilView);
            m_mainPipeline->Draw(m_renderQueue);
            m_mainPipeline->EndFrame();
        }

        m_renderQueue.clear();

        hr = m_swapChain->Present(m_settings.vsync, 0) HTHROW("failed to present swap chain");
        m_framestate = false;
    }

    void Renderer::Bind(const Shader& shader) const
    {
        if (m_framestate == false)
            XTHROW("frame not started");

        m_mainPipeline->Bind(shader);
    }

    void Renderer::Bind(const ConstantBuffer& constantBuffer, const int slot) const
    {
        if (m_framestate == false)
            XTHROW("frame not started");

        m_mainPipeline->Bind(constantBuffer, slot);
    }

    void Renderer::Bind(const Texture& texture, const int slot) const
    {
        if (m_framestate == false)
            XTHROW("frame not started");

        m_mainPipeline->Bind(texture, slot);
    }

    void Renderer::Bind(Camera& camera) const
    {
        if (m_framestate == false)
            XTHROW("frame not started");

        m_mainPipeline->Bind(camera);
    }

    void Renderer::Draw(const Drawable* drawable)
    {
        if (m_framestate == false)
            XTHROW("frame not started");

        m_renderQueue.emplace_back(drawable);
    }

    void Renderer::m_Init()
    {
        auto hr = S_OK;
        m_device->GetImmediateContext(&m_context);

        {
            ComPtr<IDXGIDevice> dxgiDevice;
            hr = m_device.As(&dxgiDevice) HTHROW("failed to get DXGI device");

            ComPtr<IDXGIAdapter> adapter;
            hr = dxgiDevice->GetAdapter(&adapter) HTHROW("failed to get DXGI adapter");

            hr = adapter->GetParent(IID_PPV_ARGS(&m_dxgiFactory)) HTHROW("failed to get DXGI factory");
        }

        m_viewport = {};
        m_viewport.Width = 800;
        m_viewport.Height = 600;
        m_viewport.MaxDepth = 1.0f;

        m_InitSwapChain();
        m_InitBuffers();
        m_InitPipelines();
    }

    void Renderer::m_InitSwapChain()
    {
        auto hr = S_OK;

        DXGI_SWAP_CHAIN_DESC1 dscd1 = {};
        dscd1.BufferCount = 2;
        dscd1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dscd1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dscd1.Width = static_cast<UINT>(m_viewport.Width);
        dscd1.Height = static_cast<UINT>(m_viewport.Height);
        dscd1.SampleDesc.Count = 1;
        dscd1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC dscfd = {};
        dscfd.Windowed = true;

        hr = m_dxgiFactory->CreateSwapChainForHwnd(
            m_device.Get(),
            m_window,
            &dscd1,
            &dscfd,
            nullptr,
            &m_swapChain
        ) HTHROW("failed to create swap chain");

        ComPtr<IDXGIFactory> dxgiFactory;
        hr = m_swapChain->GetParent(IID_PPV_ARGS(&dxgiFactory)) HTHROW("failed to get DXGI factory");
        hr = dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES) HTHROW("failed to make window association");
    }

    void Renderer::m_InitWindowStyles() const
    {
        auto style = GetWindowLongPtr(m_window, GWL_STYLE);
        auto exStyle = GetWindowLongPtr(m_window, GWL_EXSTYLE);

        style |= WS_OVERLAPPEDWINDOW;
        style &= ~WS_SIZEBOX;
        style &= ~WS_THICKFRAME;
        style &= ~WS_MAXIMIZEBOX;
        SetWindowLongPtr(m_window, GWL_STYLE, style);

        exStyle &= ~WS_EX_APPWINDOW;
        exStyle |= WS_EX_OVERLAPPEDWINDOW;
        SetWindowLongPtr(m_window, GWL_EXSTYLE, exStyle);

        SetWindowPos(m_window, HWND_TOP, 0, 0, 800, 600, SWP_FRAMECHANGED | SWP_NOMOVE);
    }

    void Renderer::m_InitBuffers()
    {
        auto hr = S_OK;

        ComPtr<ID3D11Texture2D> rtt;
        hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&rtt)) HTHROW("failed to get back buffer");
        hr = m_device->CreateRenderTargetView(rtt.Get(), nullptr, &m_renderTargetView) HTHROW("failed to create render target view");

        {
            D3D11_TEXTURE2D_DESC dstd = {};
            dstd.Width = static_cast<UINT>(m_viewport.Width);
            dstd.Height = static_cast<UINT>(m_viewport.Height);
            dstd.MipLevels = 1;
            dstd.ArraySize = 1;
            dstd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dstd.SampleDesc.Count = 1;
            dstd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

            ComPtr<ID3D11Texture2D> dst;
            hr = m_device->CreateTexture2D(&dstd, nullptr, &dst) HTHROW("failed to create depth stencil texture");
            hr = m_device->CreateDepthStencilView(dst.Get(), nullptr, &m_depthStencilView) HTHROW("failed to create depth stencil view");
        }
    }

    void Renderer::m_InitPipelines()
    {
        m_mainPipeline = std::make_unique<pipeline::MainPipeline>(m_device);
    }
}
