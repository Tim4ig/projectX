
#include "d3d11factory.hpp"

#include "exception.hpp"

namespace x::render
{
    D3D11Factory::D3D11Factory()
    {
        m_Init();
    }

    D3D11Factory::~D3D11Factory()
    {
    }


    std::unique_ptr<D3D11Renderer> D3D11Factory::CreateRenderer(HWND window)
    {
        return std::make_unique<D3D11Renderer>(m_device, window);
    }

    void D3D11Factory::m_Init()
    {
        m_InitFactory();
        m_SelectDefaultAdapter();
        m_InitDevice();
    }

    void D3D11Factory::m_InitFactory()
    {
        auto hr = S_OK;
        hr = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)) HTHROW("failed to create DXGI factory");
    }

    void D3D11Factory::m_SelectDefaultAdapter()
    {
        // D3D11 automatically selects the default adapter if nullptr is passed
        m_adapter = nullptr;
    }

    void D3D11Factory::m_InitDevice()
    {
        auto hr = S_OK;

        UINT flags = 0;

        if constexpr (core::XDEBUG)
        {
            flags |= D3D11_CREATE_DEVICE_DEBUG;
        }

        D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0
        };

        D3D_FEATURE_LEVEL selectedFeatureLevel;

        hr = D3D11CreateDevice(
            m_adapter.Get(),
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &m_device,
            &selectedFeatureLevel,
            &m_context
        ) HTHROW("failed to create device");

        {
            ComPtr<IDXGIDevice> dxgiDevice;
            hr = m_device.As(&dxgiDevice) HTHROW("failed to get DXGI device");

            ComPtr<IDXGIAdapter> adapter;
            hr = dxgiDevice->GetAdapter(&adapter) HTHROW("failed to get DXGI adapter");

            DXGI_ADAPTER_DESC dad;
            hr = adapter->GetDesc(&dad) HTHROW("failed to get DXGI adapter description");

            logger.Info("Selected adapter: ", logger.ToA(dad.Description));
        }

        logger.Info("Selected feature level: ", static_cast<unsigned int>(selectedFeatureLevel));
    }
}
