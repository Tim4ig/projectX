
#include "d3d11renderer.hpp"

#include "exception.hpp"

namespace x::render
{
    D3D11Renderer::D3D11Renderer(ComPtr<ID3D11Device> device, const HWND window) : m_device(device), m_window(window)
    {
        m_InitWindowStyles();
        m_Init();
    }

    D3D11Renderer::~D3D11Renderer()
    {
    }

    void D3D11Renderer::SetClearColor(const unsigned int rgba)
    {
        m_settings.clearColor[0] = ((rgba >> 24) & 0xFF) / 255.0f;
        m_settings.clearColor[1] = ((rgba >> 16) & 0xFF) / 255.0f;
        m_settings.clearColor[2] = ((rgba >> 8) & 0xFF) / 255.0f;
        m_settings.clearColor[3] = (rgba & 0xFF) / 255.0f;
    }

    void D3D11Renderer::Clear()
    {
        if (m_framestate == false) XTHROW("frame not started");

        m_context->ClearRenderTargetView(m_renderTargetView.Get(), m_settings.clearColor);
        m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void D3D11Renderer::SetResolution(POINT size, bool fullscreen)
    {
        if (m_framestate == true) XTHROW("frame already started");

        m_viewport.Width = size.x;
        m_viewport.Height = size.y;

        m_context->OMSetRenderTargets(0, nullptr, nullptr);
        m_renderTargetView.Reset();
        m_depthStencilView.Reset();

        auto hr = S_OK;

        if (fullscreen != m_fullscreen)
        {
            m_fullscreen = fullscreen;
            if (!m_fullscreen)
            {
                m_InitWindowStyles();
            }

            m_InitSwapChain();
        }

        m_fullscreen = fullscreen;
        hr = m_swapChain->ResizeBuffers(0, m_viewport.Width, m_viewport.Height, DXGI_FORMAT_UNKNOWN, 0) HTHROW("failed to resize buffers");

        if (!m_fullscreen)
        {
            SetWindowPos(m_window, nullptr, 0, 0, m_viewport.Width, m_viewport.Height, SWP_NOMOVE | SWP_NOZORDER);
        }

        m_InitBuffers();
    }

    void D3D11Renderer::BeginFrame()
    {
        if (m_framestate == true) XTHROW("frame already started");
        m_framestate = true;

        m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
        m_context->RSSetViewports(1, &m_viewport);
    }

    void D3D11Renderer::EndFrame()
    {
        if (m_framestate == false) XTHROW("frame already ended");
        m_framestate = false;

        auto hr = S_OK;
        hr = m_swapChain->Present(m_settings.vsync, 0) HTHROW("failed to present swap chain");
    }

    void D3D11Renderer::m_Init()
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
    }

    void D3D11Renderer::m_InitSwapChain()
    {
        auto hr = S_OK;

        if (m_swapChain)
        {
            BOOL fullscreen;
            hr = m_swapChain->GetFullscreenState(&fullscreen, nullptr) HTHROW("failed to get fullscreen state");
            if (fullscreen) hr = m_swapChain->SetFullscreenState(FALSE, nullptr) HTHROW("failed to set fullscreen state");
        }

        DXGI_SWAP_CHAIN_DESC1 dscd1 = {};
        dscd1.BufferCount = 2;
        dscd1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dscd1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dscd1.Width = m_viewport.Width;
        dscd1.Height = m_viewport.Height;
        dscd1.SampleDesc.Count = 1;
        dscd1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC dscfd = {};
        dscfd.Windowed = !m_fullscreen;

        hr = m_dxgiFactory->CreateSwapChainForHwnd(
            m_device.Get(),
            m_window,
            &dscd1,
            &dscfd,
            nullptr,
            &m_swapChain
        ) HTHROW("failed to create swap chain");
    }

    void D3D11Renderer::m_InitBuffers()
    {
        auto hr = S_OK;

        ComPtr<ID3D11Texture2D> rtt;
        hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&rtt)) HTHROW("failed to get back buffer");
        hr = m_device->CreateRenderTargetView(rtt.Get(), nullptr, &m_renderTargetView) HTHROW("failed to create render target view");

        {
            D3D11_TEXTURE2D_DESC dstd = {};
            dstd.Width = m_viewport.Width;
            dstd.Height = m_viewport.Height;
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

    void D3D11Renderer::m_InitWindowStyles()
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
}