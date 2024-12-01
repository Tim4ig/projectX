#include "texture.hpp"

namespace x::render
{
    void Texture::SetTexture(const fs::Texture& texture)
    {
        auto hr = S_OK;

        D3D11_TEXTURE2D_DESC td = {};
        td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        td.Width = texture.width;
        td.Height = texture.height;
        td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        td.MipLevels = 1;
        td.SampleDesc.Count = 1;
        td.ArraySize = 1;

        D3D11_SUBRESOURCE_DATA srd = {};
        srd.pSysMem = texture.data.get();
        srd.SysMemPitch = texture.width * 4;

        hr = m_device->CreateTexture2D(&td, &srd, &m_texture)
        HTHROW("Failed to create texture2D");
        hr = m_device->CreateShaderResourceView(m_texture.Get(), nullptr, &m_shaderResourceView) HTHROW("Failed to create shader resource view");
    }
}
