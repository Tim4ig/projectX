#pragma once

#include <dxgi1_2.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "resources/d3d11drawable.hpp"
#include "resources/d3d11shader.hpp"

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class Renderer
    {
    public:
        Renderer(const ComPtr<ID3D11Device>& device, HWND window);
        ~Renderer();

        void SetResolution(POINT size, bool fullscreen);
        void SetClearColor(unsigned int rgba);

        void Clear();
        void BeginFrame();
        void EndFrame();

        void Draw(Drawable& drawable);
        void Bind(Shader& shader);

    private:
        HWND m_window;

        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

        ComPtr<IDXGIFactory2> m_dxgiFactory;
        ComPtr<IDXGISwapChain1> m_swapChain;

        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;

        D3D11_VIEWPORT m_viewport;

        bool m_framestate = false;
        bool m_fullscreen = false;

        struct
        {
            bool vsync = false;
            float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        } m_settings;

        void m_Init();
        void m_InitSwapChain();
        void m_InitBuffers();
        void m_InitWindowStyles();
    };
}
