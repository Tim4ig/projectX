#pragma once

#include <dxgi1_2.h>
#include <d3d11.h>
#include <wrl/client.h>

#include "resources/camera.hpp"
#include "resources/drawable.hpp"
#include "resources/shader.hpp"
#include "resources/constant.hpp"

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class Renderer
    {
    public:
        Renderer(const ComPtr<ID3D11Device>& device, HWND window);
        ~Renderer();

        void SetResolution(POINT size);
        void SetClearColor(unsigned int rgba);

        void Clear();
        void BeginFrame();
        void EndFrame();

        void Draw(Drawable& drawable);

        void Bind(const Shader& shader);
        void Bind(const ConstantBuffer& constantBuffer, int slot);
        void Bind(Camera& camera);

    private:
        HWND m_window;

        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

        ComPtr<IDXGIFactory2> m_dxgiFactory;
        ComPtr<IDXGISwapChain1> m_swapChain;

        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<ID3D11DepthStencilView> m_depthStencilView;

        D3D11_VIEWPORT m_viewport;

        ConstantBuffer m_constantBuffer;

        bool m_framestate = false;

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
