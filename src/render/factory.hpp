#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
#include <memory>
#include <wrl/client.h>

#include "renderer.hpp"
#include "resources/drawable.hpp"
#include "resources/shader.hpp"

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class Factory
    {
    public:
        Factory();

        std::unique_ptr<Renderer> CreateRenderer(HWND window);
        std::unique_ptr<Drawable> CreateDrawable();
        std::unique_ptr<Shader> CreateShader();

    private:
        ComPtr<ID3D11Debug> m_debug;
        ComPtr<ID3D11Device> m_device;
        ComPtr<ID3D11DeviceContext> m_context;

        ComPtr<IDXGIFactory2> m_dxgiFactory;
        ComPtr<IDXGIAdapter> m_adapter;

        void m_Init();
        void m_InitFactory();
        void m_SelectDefaultAdapter();
        void m_InitDevice();
    };
}
