
#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
#include <memory>
#include <wrl/client.h>

#include "d3d11renderer.hpp"

using Microsoft::WRL::ComPtr;

namespace x::render
{
    class D3D11Factory
    {
    public:
        D3D11Factory();
        ~D3D11Factory();

        std::unique_ptr<D3D11Renderer> CreateRenderer(HWND window);
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
