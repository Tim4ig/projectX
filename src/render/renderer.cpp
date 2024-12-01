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

    void Renderer::SetClearColor(const unsigned int rgba) const
    {
        m_mainPipeline->SetClearColor(rgba);
    }

    void Renderer::Clear() const
    {
        if (m_framestate == false)
            XTHROW("frame not started");

        m_mainPipeline->Clear();
    }

    void Renderer::SetResolution(const POINT size)
    {
        if (m_framestate == true)
            XTHROW("frame already started");

        m_viewport.Width = static_cast<FLOAT>(size.x);
        m_viewport.Height = static_cast<FLOAT>(size.y);

        m_mainPipeline->Resize(size);

        SetWindowPos(m_window, nullptr, 0, 0, static_cast<int>(m_viewport.Width), static_cast<int>(m_viewport.Height), SWP_NOMOVE | SWP_NOZORDER);
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

        m_framestate = false;

        {
            m_mainPipeline->BeginFrame();
            m_mainPipeline->Draw(m_renderQueue);
            m_mainPipeline->EndFrame();
        }

        m_renderQueue.clear();

        auto hr = S_OK;
        hr = m_swapChain->Present(m_settings.vsync, 0) HTHROW("failed to present swap chain");
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

    void Renderer::m_InitPipelines()
    {
        m_mainPipeline = std::make_unique<pipeline::MainPipeline>(m_device, m_swapChain);
    }
}
